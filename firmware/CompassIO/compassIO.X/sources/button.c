/*
 * File:   button.c
 * Author: bocal
 *
 * Created on July 27, 2018, 11:35 AM
 */

#include "types.h"

void init_button()
{
    INTCONbits.INT1EP = 0; //0->lorsqu'on entre, 1 lorsqu'on sort l'interrupt se produit

    // INT1 - Button
    IPC1bits.INT1IP = 1;
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
	I2C1_Init();
	Init_Timer4();
    INTCONbits.MVEC = 1; // Enable multi interrupts
    __builtin_enable_interrupts();
    delayms(1000);
    ft_putendl("Start");
	init_mag();
    /* GPS */
    LATDbits.LATD6 = 0; // Set nRST GPS
    LATDbits.LATD5 = 0; // Set ON_OFF GPS
    rst = 0;
    on_off = 0;
    gps = 0;
}

void global_off()
{
    U1MODEbits.ON = 0; // Disable UART1 Module
    U2MODEbits.ON = 0; // Disable UART2 Module
    I2C1CONbits.ON = 0; // Disable I2C1
    SPI2CONbits.ON = 0; // Disable SPI2 Flash Memory
    T2CONbits.ON = 0;  // Disable Timer2 Servomotor
}

void __ISR(_EXTERNAL_1_VECTOR, IPL1SRS) MainButtonHandler(void) {
    if (INTCONbits.INT1EP == 1) { // Button Released
		if (devicePowered && countTime > FIVE_SEC)
        {
            ft_putendl("Enter in calibration mode");
			set_timer4(TIMER4_100MS);
			IsCalMode = TRUE;
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
        INTCONbits.INT1EP = 0; // Get button pushing instead of getting release
    } else { // Button pressed
        countTimeEnable = TRUE;
        INTCONbits.INT1EP = 1; // Active button release mode
    }
    IFS0bits.INT1IF = 0; // Reset to 0 Interrupt INT1
}
