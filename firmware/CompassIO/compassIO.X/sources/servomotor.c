/*
 * File:   servomotor.c
 * Author: bocal
 *
 * Created on July 10, 2018, 4:42 PM
 */

#include "types.h"

void ServoMotorSetAngle(u8 dutyCycle)
{
	if (dutyCycle > 180)
		dutyCycle = 180;
	OC1RS = (PR2 + 1) * ((float)dutyCycle/1890 + 0.035); //0.035 -> 1ms Offset
}

void init_servo()
{
	TRISDbits.TRISD0 = 0;
	LATDbits.LATD0 = 0;

	T2CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR2 = 0;                // Clean the timer register
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	T2CONbits.TCKPS = 0b101; // Set scaler 1:16
	PR2 = PBCLK/32/50;    // Setup the period > Periph. Bus Clk (set by user) / scaler > sec | 20ms

	T2CONbits.ON = 1;

	OC1CONbits.OCM = 0b110;
	ServoMotorSetAngle(90);
	OC1CONbits.ON = 1;
}
