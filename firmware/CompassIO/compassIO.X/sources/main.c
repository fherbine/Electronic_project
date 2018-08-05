/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

char buffBT[500];
struct s_taskflag thisTaskFlag;

#define NEWLINE 13

/* UART -> Bluetooth */
void __ISR(_UART1_VECTOR, IPL2SRS) UART1Handler(void) {
	// Reception
	if (IFS0bits.U1RXIF) {
		IFS0CLR = U1RX_IFS1;
		// Store input in buffer
		u32 dest_len = ft_strlen(buffBT);
		buffBT[dest_len] = UART1_Get_Data_Byte();
		UART2_Send_Data_Byte(buffBT[dest_len]);
		buffBT[dest_len + 1] = '\0';
		if (buffBT[dest_len] == NEWLINE) {
			buffBT[dest_len] = '\0';
			parser_gps_bluetooth(buffBT);
			ft_bzero(buffBT, 500);
		}
	}
	// Transmit
	if (IFS0bits.U1TXIF)
		IFS0CLR = U1TX_IFS1;
	// Error
	if (IFS0bits.U1EIF)
		IFS0CLR = U1E_IFS1;
}

/* UART -> GPS/Debug */
void __ISR(_UART2_VECTOR, IPL2SRS) UART2Handler(void) {
	// Reception
	if (IFS1bits.U2RXIF) {
		IFS1CLR = U2RX_IFS1;
		UART1_Send_Data_Byte(UART2_Get_Data_Byte());
	}
	// Transmit
	if (IFS1bits.U2TXIF)
		IFS1CLR = U2TX_IFS1;
	// Error
	if (IFS1bits.U2EIF)
		IFS1CLR = U2E_IFS1;
}

u16 countTime = 0;
u8  countTimeEnable = FALSE;
u8  devicePowered = FALSE;
unsigned char  rst = 0, on_off = 0, gps = 0;
unsigned short gpsTmp = 0;

u8 powerOnProcess = FALSE;
u8 powerOffProcess = FALSE;

void	gps_power_on(void)
{
	if (gpsTmp == 100 && !rst) // 100ms after PIC power on
	{
		LATDbits.LATD6 = 1;
		rst = 1;
	}
	else if (gpsTmp == 110 && !on_off)
	{
		LATDbits.LATD5 = 1;
		on_off = 1;
	}
	else if (gpsTmp == 310 && on_off)
	{
		LATDbits.LATD5 = 0;
		on_off = 0;
	}
	else if (gpsTmp > 310)
	{
		gpsTmp = 0;
		gps = 1;
                powerOnProcess = FALSE;
	}
}

void	gps_power_off(void)
{
	if (gpsTmp == 0)
		LATDbits.LATD5 = 1;
	else if (gpsTmp == 200)
        {
           	LATDbits.LATD5 = 0;
                powerOffProcess = FALSE;
        }
	gpsTmp++;
}

#define MAX_U16 0xFFFF

/* Calibrate MAG3110 */
s16	x_min = 0x7FFF;
s16	x_max = 0x8000;
s16	y_min = 0x7FFF;
s16	y_max = 0x8000;

s16 offset_x = 0;
s16 offset_y = 0;

float x_scale = 0;
float y_scale = 0;

s32 TimerCalMode = 0;

void __ISR(_TIMER_3_VECTOR, IPL1) Timer3Handler(void) {
	IFS0bits.T3IF = 0;
    /* Power Off 2 Sec */
    if (countTimeEnable && countTime < MAX_U16)
        countTime++;
    /* ON OFF GPS Sequence */
    if (powerOnProcess)
        gps_power_on();
    if (powerOffProcess)
        gps_power_off();
	if (thisTaskFlag.CalMag)
		TimerCalMode++;
    gpsTmp++;
}

void storeMagData(s16 max_x, s16 min_x, s16 max_y, s16 min_y) {
	erase_sector(STORE_MAG_MAX_X);
	delayms(85);
	write_data(STORE_MAG_MAX_X, max_x, 2);
	delayms(85);
	erase_sector(STORE_MAG_MIN_X);
	delayms(85);
	write_data(STORE_MAG_MIN_X, min_x, 2);
	delayms(85);
	erase_sector(STORE_MAG_MAX_Y);
	delayms(85);
	write_data(STORE_MAG_MAX_Y, max_y, 2);
	delayms(85);
	erase_sector(STORE_MAG_MIN_Y);
	delayms(85);
	write_data(STORE_MAG_MIN_Y, min_y, 2);
	delayms(85);
}

void calibrateMag(s16 x, s16 y)
{
	if(x < x_min)
	  x_min = x;
	if(x > x_max)
	  x_max = x;
	if(y < y_min)
	  y_min = y;
	if(y > y_max)
	  y_max = y;
	if(TimerCalMode > 5000) // Exit calibration
	{
		thisTaskFlag.CalMag = FALSE;
		LATFbits.LATF1 = 0;
		offset_x = (x_min + x_max) / 2;
		offset_y = (y_min + y_max) / 2;
		x_scale = 1.0/(float)(x_max - x_min);
		y_scale = 1.0/(float)(y_max - y_min);
		storeMagData(offset_x, offset_y, x_scale, y_scale);
		/* Back Timer4 on 500ms frequency */
		PR4 = PBCLK/256/2;
		TMR4 = 0;
		/* ======================= */
		ft_putstr("x_min: ");
		ft_putnbr_base(x_min, 10);
		ft_putstr(" x_max: ");
		ft_putnbr_base(x_max, 10);
		ft_putstr(" y_min: ");
		ft_putnbr_base(y_min, 10);
		ft_putstr(" y_max: ");
		ft_putnbr_base(y_max, 10);
		ft_putstr("\n\r");
		ft_putstr(" offset x: ");
		ft_putnbr_base(offset_x, 10);
		ft_putstr(" offset y: ");
		ft_putnbr_base(offset_y, 10);
		ft_putstr("\n\r");
		ft_putstr(" x_scale: ");
		ft_putfloat(x_scale);
		ft_putstr(" y_scale: ");
		ft_putfloat(y_scale);
		ft_putstr("\n\r");
	}
}

#define DEG_PER_RAD (180.0/3.14159265358979)

s16 readHeading(s16 x, s16 y)
{
	float xf = (float) x * 1.0f;
	float yf = (float) y * 1.0f;

	//Calculate the heading
	return (atan2(-yf*y_scale, xf*x_scale) * DEG_PER_RAD);
}


void __ISR(_TIMER_4_VECTOR, IPL6) Timer4Handler(void) {
    IFS0bits.T4IF = 0;
	thisTaskFlag.Mag = 1;
}

void Init_Timer4()
{
  T4CON = 0;               // 0 on every bit, (timer stop, basic config)
  TMR4 = 2;                // Clean the timer register

  IPC4bits.T4IP = 6; // Set priority
  IPC4bits.T4IS = 1; // Set subpriority
  IFS0bits.T4IF = 0;       // Clear interrupt status flag
  IEC0bits.T4IE = 1; // Enable interrupts

  T4CONbits.TCKPS = 0b111; // Set scaler 1:256
  PR4 = PBCLK/256/2;    // Setup the period
  T4CONbits.ON = 1;
}

void global_init()
{
    __builtin_disable_interrupts();
    UART1_Init(_8N, 0, UART_RX_TX_ON);
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    init_servo();
    Init_SPI2();
    I2C1_Init();
    Init_Timer4();
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    init_mag();
    /* GPS */
    LATDbits.LATD6 = 0; // Set nRST GPS
    LATDbits.LATD5 = 0; // Set ON_OFF GPS
    rst = 0;
    on_off = 0;
    gps = 0;
	/* MAG - OFFSET */
	max_x = (s16)read_data(STORE_MAG_MAX_X, 2);
	delayms(85);
	min_x = (s16)read_data(STORE_MAG_MIN_X, 2);
	delayms(85);
	max_y = (s16)read_data(STORE_MAG_MAX_Y, 2);
	delayms(85);
	min_y = (s16)read_data(STORE_MAG_MIN_Y, 2);
	offset_x = (x_min + x_max) / 2;
	offset_y = (y_min + y_max) / 2;
	x_scale = 1.0/(float)(x_max - x_min);
	y_scale = 1.0/(float)(y_max - y_min);
  delayms(1000);
  ft_putendl("Start");
	ft_putnbr_base((s16)offset_x, 10);
	ft_putstr(" ");
	ft_putnbr_base((s16)offset_y, 10);
	ft_putstr(" ");
	ft_putnbr_base((s16)x_scale, 10);
	ft_putstr(" ");
	ft_putnbr_base((s16)y_scale, 10);
	ft_putstr(" ");
}

void global_off()
{
    U1MODEbits.ON = 0; // Disable UART1 Module
    U2MODEbits.ON = 0; // Disable UART2 Module
    I2C1CONbits.ON = 0; // Disable I2C1
    SPI2CONbits.ON = 0; // Disable SPI2 Flash Memory
    T2CONbits.ON = 0;  // Disable Timer2 Servomotor
}

#define ONE_HALF_SEC 1500
#define FIVE_SEC 5000

void __ISR(_EXTERNAL_1_VECTOR, IPL1) MainButtonHandler(void) {
    if (INTCONbits.INT1EP == 1) { // Button Released
	if (devicePowered && countTime > FIVE_SEC)
        {
            ft_putendl("Enter in calibration mode");
            /* Set Timer4 on 100ms frequency */
            PR4 = PBCLK/256/10;
            TMR4 = 0;
            /* ============================== */
						// Clear offset
						offset_x = 0;
						offset_y = 0;
            thisTaskFlag.CalMag = TRUE;
            LATFbits.LATF1 = 1;
            TimerCalMode = 0;
        }
		else if (devicePowered && countTime > ONE_HALF_SEC)
        {
            powerOffProcess = TRUE;
            gpsTmp = 0;
            ft_putendl("GLOBAL POWER OFF");
            global_off();
            devicePowered = FALSE;
        }
        else
        {
            if (devicePowered)
            {
                ft_putendl("destination switch");
								ft_putnbr_base(countTime, 10);
            }
            else
            {
                powerOnProcess = TRUE;
                gpsTmp = 0;
                global_init();
                ft_putendl("first time = GLOBAL POWER ON");
                devicePowered = TRUE;
            }
        }
        countTime = 0;
        countTimeEnable = FALSE;
        INTCONbits.INT1EP = 0;
        // Get button pushing instead of getting release
    } else { // Button pressed
        countTimeEnable = TRUE;
        INTCONbits.INT1EP = 1; // Active button1 release mode
    }
    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT1
}

void init_button()
{
    INTCONbits.INT1EP = 0; //0->lorsqu'on entre, 1 lorsqu'on sort l'interrupt se produit

    // INT1 - Button
    IPC1bits.INT1IP = 1;
    IPC1bits.INT1IS = 0;
    IFS0bits.INT1IF = 0;
    IEC0bits.INT1IE = 1;
}

void Mag(s16 x, s16 y) {
	if (devicePowered) {
		LATFbits.LATF1 ^= 1;
		s16 degrees = (int)readHeading(x - offset_x, y - offset_y);
        ft_putnbr_base(degrees, 10);
        ft_putstr("\n\r");
		if (degrees < -90 || degrees > 90)
			degrees = (degrees > 90) ? 0 : 180;
		else
			degrees = 90 - degrees;
		thisTaskFlag.Mag = 0;
	}
}

void main()
{
    TRISFbits.TRISF1 = 0; // LED writable
    LATFbits.LATF1 = 0;
    TRISDbits.TRISD6 = 0; // RD6 is an output -> nRST GPS
    LATDbits.LATD6 = 1;
    TRISDbits.TRISD5 = 0; // RD5 is an output -> ON_OFF GPS

    __builtin_disable_interrupts();
    Init_Delay();
    init_button();
    init_servo();
    ServoMotorSetAngle(180);
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
	thisTaskFlag.Mag = 0;
	thisTaskFlag.CalMag = 0;
	s16 mag_x = 0.0, mag_y = 0.0, mag_z = 0.0;
    while (1) {
		if (thisTaskFlag.Mag == 1) {
			readMag(&mag_x, &mag_y, &mag_z);
			Mag(mag_x, mag_y);
			if (thisTaskFlag.CalMag == 1) {
				calibrateMag(mag_x, mag_y);
			}
		}
	}
}

/*
 *
     delayms(100);
    store_double(0x030000, 999);
    delayms(85);
    read_data(0x030000, 8);
    Read status register
    //read_status_register();
 */
