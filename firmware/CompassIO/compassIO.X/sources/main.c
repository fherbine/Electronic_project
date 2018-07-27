/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

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

    while (1) ;
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
