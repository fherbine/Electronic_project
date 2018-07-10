/*
 * File:   servomotor.c
 * Author: bocal
 *
 * Created on July 10, 2018, 4:42 PM
 */

#include "types.h"

void __ISR(_TIMER_2_VECTOR, IPL1) Timer2Handler(void) {
    TMR2 = 0;
	LATDbits.LATD0 ^= 1;
    IFS0bits.T2IF = 0;
}

void __ISR(_OUTPUT_COMPARE_1_VECTOR, IPL1) OC2Handler(void) {
    IFS0bits.OC1IF = 0;
}

void SetDutyCycle(unsigned char dutyCycle)
{
	OC1RS = PBCLK/16/100/dutyCycle;
	OC1R = PBCLK/16/100/dutyCycle;
}

// http://umassamherstm5.org/tech-tutorials/pic32-tutorials/pic32mx220-tutorials/pwm
void init_servo()
{
	TRISDbits.TRISD0 = 0;
	LATDbits.LATD0 = 0;
	T2CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR2 = 0;                // Clean the timer register
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	T2CONbits.TCKPS = 0b100; // Set scaler 1:16
	PR2 = PBCLK/16/1000;    // Setup the period > Periph. Bus Clk (set by user) / scaler > sec | 20ms

	T2CONbits.T32 = 1;

    IPC2bits.T2IP = 1; // Set priority
    IPC2bits.T2IS = 2; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts
	T2CONbits.ON = 1;

	OC1CONbits.OCM = 0b110;
	SetDutyCycle(10);
	OC1CONbits.ON = 1;
}

//int main()
//{
//	TRISDbits.TRISD8 = 1;
//	unsigned char count = 40;
//	while(1)
//	{
//		SetDutyCycle(count);
//		if (PORTDbits.RD8 == 0)
//		{
//			count += 1;
//			while (PORTDbits.RD8 == 0);
//		}
//	}
//}
