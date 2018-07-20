/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

char buffBT[500];

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
		LATFbits.LATF1 ^= 1;
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
		LATFbits.LATF1 ^= 1;
	}
	// Transmit
	if (IFS1bits.U2TXIF)
		IFS1CLR = U2TX_IFS1;
	// Error
	if (IFS1bits.U2EIF)
		IFS1CLR = U2E_IFS1;
}

u16 countTime = 0;
u8 countTimeEnable = FALSE;
u8  devicePowered = FALSE;
#define MAX_U16 0xFFFF

void __ISR(_TIMER_3_VECTOR, IPL1) PowerOff2secHandler(void) {
    if (countTimeEnable && countTime < MAX_U16)
        countTime++;
    IFS0bits.T3IF = 0;
}

#define ONE_HALF_SEC 1500

void __ISR(_EXTERNAL_1_VECTOR, IPL6) MainButtonHandler(void) {
    if (INTCONbits.INT1EP == 1) { // Button Released
        if (devicePowered && countTime > ONE_HALF_SEC)
        {
            ft_putendl("GLOBAL POWER OFF");
            global_off();
            devicePowered = FALSE;
        }
        else
        {
            if (devicePowered)
            {
                ft_putendl("destination switch");
            }
            else
            {
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
        INTCONbits.INT1EP = 1; // Active button release mode
    }
    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT1
}

void init_button()
{
    INTCONbits.INT1EP = 0; //0->lorsqu'on entre, 1 lorsqu'on sort l'interrupt se produit

    // INT1 - Button
    IPC1bits.INT1IP = 6;
    IPC1bits.INT1IS = 0;
    IFS0bits.INT1IF = 0;
    IEC0bits.INT1IE = 1;
}

void global_init()
{
    __builtin_disable_interrupts();
    UART1_Init(_8N, 0, UART_RX_TX_ON);
    UART2_Init(_8N, 0, UART_RX_TX_ON);
    init_servo();
    Init_SPI2();
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    delayms(1000);
    ft_putendl("Start");
}

void global_off()
{
    U1MODEbits.ON = 0; // Disable UART1 Module
    U2MODEbits.ON = 0; // Disable UART2 Module
    I2C1CONbits.ON = 0; // Disable I2C1
    SPI2CONbits.ON = 0; // Disable SPI2 Flash Memory
    T2CONbits.ON = 0;  // Disable Timer2 Servomotor
}

void main()
{
    LATFbits.LATF1 = 0;
    __builtin_disable_interrupts();
    Init_Delay();
    init_button();
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();

    while (1) ;
}

/*
     delayms(100);
    store_double(0x030000, 999);
    delayms(85);
    read_data(0x030000, 8);
    Read status register
    //read_status_register();
 */