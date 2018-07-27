/*
 * File:   main.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

u8  devicePowered = FALSE;
u16 countTime = 0;
u8  countTimeEnable = FALSE;
unsigned char  rst = 0, on_off = 0, gps = 0;
unsigned short gpsTmp = 0;

u8 powerOnProcess = FALSE;
u8 powerOffProcess = FALSE;

/* Calibrate MAG3110 */
s16	x_min = 0x7FFF, x_max = 0x8000, y_min = 0x7FFF, y_max = 0x8000, offset_x = 0, offset_y = 0;

float x_scale = 0, y_scale = 0;

s32 TimerCalMode = 0;
u8 IsCalMode = FALSE;

s16 mag_x = 0.0;
s16 mag_y = 0.0;

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
