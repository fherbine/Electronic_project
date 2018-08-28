#include "types.h"

struct s_data data;
struct s_taskflag thisTaskFlag;

#define NEWLINE '#'

void HandleGPS(struct s_data *data_s);
char buffBT[500];


/* UART -> GPS */
void __ISR(_UART1_VECTOR, IPL1) UART1Handler(void) {
	// Reception
	if (IFS1bits.U1RXIF) {
		IFS1bits.U1RXIF = 0;
		HandleBluetooth(&data);
		thisTaskFlag.Bluetooth = TRUE;
	}
}

/* UART -> Bluetooth/Debug */
void __ISR(_UART2_VECTOR, IPL1) UART2Handler(void) {
	// Reception
	if (IFS1bits.U2RXIF) {
		IFS1bits.U2RXIF = 0;
		thisTaskFlag.GPS = TRUE;
	}
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
	if (gpsTmp % 100 == 0)
		LATBbits.LATB2 ^= 1;													//// POWER ON LED
	if (gpsTmp == 100 && !rst) // 100ms after PIC power on
	{
		LATBbits.LATB3 = 1;
		rst = 1;
	}
	else if (gpsTmp == 110 && !on_off)
	{
		LATBbits.LATB5 = 1;
		on_off = 1;
	}
	else if (gpsTmp == 310 && on_off)
	{
		LATBbits.LATB5 = 0;
		on_off = 0;
	}
	else if (gpsTmp > 310)
	{
		gpsTmp = 0;
		gps = 1;
		powerOnProcess = FALSE;
		///////////////////////////////////////////////////////////////////////////////////// SHOULD ENABLE BT HERE
	}
}

void	gps_power_off(void)
{
	gps = 0;
	if (gpsTmp == 0)
		LATBbits.LATB5 = 1;
	else if (gpsTmp == 200)
	{
		LATBbits.LATB5 = 0;
		powerOffProcess = FALSE;
	}
	gpsTmp++;																	/// WARNING
}

#define MAX_U16 0xFFFF

/* Calibrate MAG3110 */
s16	x_min;
s16	x_max;
s16	y_min;
s16	y_max;

s16 offset_x = 0;
s16 offset_y = 0;

float x_scale = 0;
float y_scale = 0;

s32 TimerCalMode = 0;

u16 counter = 0;

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
	counter = (counter < 0xFFFF) ? counter + 1 : 0;
    gpsTmp = (gpsTmp <= 500) ? gpsTmp + 1 : 500 ;			//overflow
}

void blink_distance(struct s_data *datas)
{
	if (counter % ((6 * datas->current_distance * 100) / datas->init_distance + 100))
		LATBbits.LATB2 ^= 1;
}

void storeMagData(s16 x_max, s16 x_min, s16 y_max, s16 y_min) {
	s16 x_tab[2] = {x_max, x_min};
	s16 y_tab[2] = {y_max, y_min};
	erase_sector(STORE_MAG_X);
	delayms(85);
	store_several_datas(STORE_MAG_X, x_tab, 2, 2);
	delayms(85);
	erase_sector(STORE_MAG_Y);
	delayms(85);
	store_several_datas(STORE_MAG_Y, y_tab, 2, 2);
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
		LATBbits.LATB2 = 0;
		offset_x = (x_min + x_max) / 2;
		offset_y = (y_min + y_max) / 2;
		x_scale = 1.0/(float)(x_max - x_min);
		y_scale = 1.0/(float)(y_max - y_min);
		/* Back Timer4 on 500ms frequency */
		PR4 = PBCLK/256/2;
		TMR4 = 0;
		/* ======================= */
		storeMagData(x_max, x_min, y_max, y_min);
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

void Mag(s16 x, s16 y, struct s_data *datas) {
	if (devicePowered) {
		s16 degrees = (int)readHeading(x - offset_x, y - offset_y);
		if (datas->dest_coord.completed == TRUE && datas->current_coord.completed == TRUE)
		{
			degrees += get_direction(datas->current_coord.lat, datas->current_coord.lon, datas->dest_coord.lat, datas->dest_coord.lon);
			if (degrees < -90 || degrees > 90)
				degrees = (degrees > 90) ? 0 : 180;
			else
				degrees = 90 - degrees;
			if (thisTaskFlag.displayDist == TRUE)
				ServoMotorSetAngle(degrees);
		}
		thisTaskFlag.Mag = 0;
	}
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

void mag_offset_init(void)
{
	read_id();
	/* MAG - OFFSET */
	x_max = (s16)read_index_data(STORE_MAG_X, 2, 0);
    delayms(85);
	x_min = (s16)read_index_data(STORE_MAG_X, 2, 1);
    delayms(85);
	y_max = (s16)read_index_data(STORE_MAG_Y, 2, 0);
    delayms(85);
	y_min = (s16)read_index_data(STORE_MAG_Y, 2, 1);
	delayms(85);
	/* ft_putstr("mag x ");
	ft_putbinary(x_max);
	ft_putstr(" - ");
	ft_putbinary(x_min);
	ft_putendl("");
	ft_putstr("mag y ");
	ft_putbinary(y_max);
	ft_putstr(" - ");
	ft_putbinary(y_min);
	ft_putendl("");*/
}
u8 dest_selected = 0;
void global_init()
{
    __builtin_disable_interrupts();
    UART1_Init(_8N, 0, UART_RX_TX_ON);
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    init_servo();
    Init_SPI1();
    I2C1_Init();
    Init_Timer4();
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    init_mag();
    /* GPS */
    LATBbits.LATB5 = 0;
    LATBbits.LATB3 = 0;
    rst = 0;
    on_off = 0;
    gps = 0;
	ft_bzero(buffBT, 500);
    mag_offset_init();
	init_struct_datas(&data);
	dest_selected = 0;
	/*	data->dest_coord.lat = (float)(((u32)read_data(STORE_DEST_LAT_X1000, 4)) / 1000);
		delayms(85);
		data->dest_coord.lon = (float)(((u32)read_data(STORE_DEST_LONG_X1000, 4)) / 1000);
		if (data->dest_coord.lat != 0.0 && data->dest_coord.lon != 0.0) {
			data->dest_coord.completed = TRUE;
		}*/                                                                 // >>>>>>>> COMMENT
    offset_x = (x_min + x_max) / 2;
    offset_y = (y_min + y_max) / 2;
    x_scale = 1.0/(float)(x_max - x_min);
    y_scale = 1.0/(float)(y_max - y_min);
    delayms(50);
}

void global_off()
{
    U1MODEbits.ON = 0; // Disable UART1 Module
    U2MODEbits.ON = 0; // Disable UART2 Module
    I2C1CONbits.ON = 0; // Disable I2C1
    SPI2CONbits.ON = 0; // Disable SPI2 Flash Memory
    T2CONbits.ON = 0;  // Disable Timer2 Servomotor
}

void store_datas(void)
{
	erase_sector(STORE_DEST_LAT_X1000);
	delayms(200);
	write_data(STORE_DEST_LAT_X1000, ((int)(data.dest_coord.lat * 1000)), 4);
	delayms(85);
	erase_sector(STORE_DEST_LONG_X1000);
	delayms(200);
	write_data(STORE_DEST_LONG_X1000, ((int)(data.dest_coord.lon * 1000)), 4);
	delayms(85);
}

#define ONE_HALF_SEC 1500
#define FIVE_SEC 5000


void __ISR(_EXTERNAL_1_VECTOR, IPL1) MainButtonHandler(void) {
	if (INTCONbits.INT1EP == 0) { // Button Released
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
			x_min = 0x7FFF;														//// ?
			x_max = 0x8000;														//// ?
			y_min = 0x7FFF;														//// ?
			y_max = 0x8000;														//// ?
			thisTaskFlag.CalMag = TRUE;
			LATBbits.LATB2 = 1;
			TimerCalMode = 0;
		}
		else if (devicePowered && countTime > ONE_HALF_SEC)
		{
			powerOnProcess = FALSE;
			powerOffProcess = TRUE;
			gpsTmp = 0;
			ft_putendl("GLOBAL POWER OFF");
			//global_off();										>>>>>>>>>>>>>>> CRASH !!!
			devicePowered = FALSE;
			dest_selected = 0;
		}
		else if(countTime > 10)
		{
			if (devicePowered  && data.store_data && !dest_selected)
			{
				ft_putendl("start");
				store_datas();
				ft_putendl("okok");
				data.store_data = FALSE;
				dest_selected = 1;
			}
			else if (devicePowered && !dest_selected)
			{
				ft_putendl("start");
				dest_selected = 1;
			}
			else if (devicePowered && dest_selected)
			{
			    ft_putendl("destination switch");
				thisTaskFlag.switchPos = TRUE;
			}
			else
			{
			    powerOnProcess = TRUE;
				powerOffProcess = FALSE;
			    gpsTmp = 0;
			    global_init();
			    ft_putendl("first time = GLOBAL POWER ON");
			    devicePowered = TRUE;
			}
		}
			countTime = 0;
			countTimeEnable = FALSE;
			INTCONbits.INT1EP = 1;
			// Get button pushing instead of getting release
		} else { // Button pressed
			countTimeEnable = TRUE;
			INTCONbits.INT1EP = 0; // Active button1 release mode
		}
		IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT1
}

void init_button()
{
	INTCONbits.INT1EP = 1; //1->lorsqu'on entre, 0 lorsqu'on sort l'interrupt se produit*/

	// INT1 - Button
	IPC1bits.INT1IP = 1;
	IPC1bits.INT1IS = 0;
	IFS0bits.INT1IF = 0;
	IEC0bits.INT1IE = 1;
}

void HandleBluetooth(struct s_data *data_s) {
	// Store input in buffer
	u32 dest_len = ft_strlen(buffBT);
	u8 res = 0;
	buffBT[dest_len] = UART1_Get_Data_Byte();
	UART2_Send_Data_Byte(buffBT[dest_len]);
	buffBT[dest_len + 1] = '\0';
	if (buffBT[dest_len] == NEWLINE) {
		buffBT[dest_len] = '\0';
		//ft_putendl(buffBT);
		//ft_putnbr_base(IFS1bits.U2RXIF, 10);
		res = parser_gps_bluetooth(buffBT, data_s);
		ft_putnbr_base(data_s->store_data, 10);
		ft_putendl("<< toto");
		ft_putnbr_base(dest_selected, 10);
		ft_putendl("<< toto");
		ft_bzero(buffBT, 500);
	}
}

int res = -1;
char buffGPS[500];

void HandleGPS(struct s_data *data_s) {
	u32 dest_len = ft_strlen(buffGPS);

	buffGPS[dest_len] = UART2_Get_Data_Byte();
	buffGPS[dest_len + 1] = '\0';

	if (dest_len > 0 && buffGPS[dest_len - 1] == 13 && buffGPS[dest_len] == 10)
	{
		if (!ft_strncmp(buffGPS, "$GPRMC,", 7)) {
			buffGPS[dest_len - 1] = '\0';
			buffGPS[dest_len] = '\0';
			res = parse_nmea_gps(buffGPS, data_s);
			if (res == 1)
			{
				ft_putfloat(data_s->current_coord.lat);
				UART2_Send_Data_Byte('-');
				ft_putfloat(data_s->current_coord.lon);
			}
		}
		dest_len = 0;
		ft_bzero(buffGPS, 500);
	}
	//LATBbits.LATB1 ^= 1;
}

void init_task_flags(void)
{
	thisTaskFlag.Bluetooth = FALSE;
	thisTaskFlag.CalMag = FALSE;
	thisTaskFlag.GPS = FALSE;
	thisTaskFlag.Mag = FALSE;
	thisTaskFlag.switchPos = FALSE;
	thisTaskFlag.displayDist = FALSE;
}

void main()
{
	//struct s_data *data;
	SET_USER_LED(OUTPUT); // LED writable
	USER_LED_ON(FALSE);
	TRISBbits.TRISB5 = 0; // RD6 is an output -> ON_OFF GPS
	LATBbits.LATB5 = 0;
	TRISBbits.TRISB3 = 0; // RD5 is an output -> nRST GPS
    LATBbits.LATB3 = 1;

	set_pps();
	init_task_flags();
	s16 mag_x = 0.0, mag_y = 0.0, mag_z = 0.0;

	__builtin_disable_interrupts();
	Init_Delay();
	init_button();
	init_servo();
	ServoMotorSetAngle(180);
	INTCONbits.MVEC = 1; // Enable multi interrupts
	__builtin_enable_interrupts();
        //ft_putendl("start");
	// Set each element of the struc to NULL
	//ft_memset(&thisTaskFlag, 0, sizeof(thisTaskFlag)); // USELESS ?
	//ft_memset(&data, 0, sizeof(data));                 // USELESS ?
	while (1) {
		if (thisTaskFlag.displayDist == FALSE && data.current_coord.completed == TRUE)
			thisTaskFlag.displayDist = TRUE;
		if (thisTaskFlag.Mag == 1) {
			readMag(&mag_x, &mag_y, &mag_z);
			Mag(mag_x, mag_y, &data);													/// ?
			if (thisTaskFlag.CalMag == 1) {
				calibrateMag(mag_x, mag_y);
			}
		}
		if (thisTaskFlag.Bluetooth == 1) {
			thisTaskFlag.Bluetooth = 0;
		}
		if (thisTaskFlag.GPS = 1) {
			HandleGPS(&data);
			thisTaskFlag.GPS = 0;
		}
		if (thisTaskFlag.switchPos == TRUE)
		{
			switch_position(&data);
			thisTaskFlag.switchPos = FALSE;
		}
		if (thisTaskFlag.displayDist == TRUE)
			blink_distance(&data);

		if (thisTaskFlag.displayDist == TRUE && data.current_distance <= 10){		// the walk is over 10m near from the final destination
			thisTaskFlag.displayDist = FALSE;
			LATBbits.LATB2 = 1;
		}
	}
}
