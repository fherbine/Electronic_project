/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 29, 2018, 4:05 PM
 */

#include <p32xxxx.h>
#include "xc.h"
#include "sys/attribs.h"

#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = HS

#define SYSCLK (8000000/2*20/1) // 80Mhz
#define PBCLK (SYSCLK/8) // 10Mhz


/*
 * 
 */

void __ISR(_TIMER_2_VECTOR, IPL1) Timer2Handler(void) {
    TMR2 = 0;
	LATFbits.LATF1 ^= 1;
    IFS0bits.T2IF = 0;
}

void SetDutyCycle(unsigned char dutyCycle)
{
	OC1RS = PBCLK/256/5/dutyCycle;
	OC1R = PBCLK/256/5/dutyCycle;
}

void Degres(unsigned char degres)
{

}

int main()
{
	__builtin_disable_interrupts();
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;
	T2CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR2 = 0;                // Clean the timer register
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	T2CONbits.TCKPS = 0b111; // Set scaler 1:256
	PR2 = PBCLK/256/50;    // Setup the period > Periph. Bus Clk (set by user) / scaler > sec | 20ms
   
    IPC2bits.T2IP = 1; // Set priority
    IPC2bits.T2IS = 2; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts
	T2CONbits.ON = 1;

	OC1CONbits.OCM = 0b110;
	SetDutyCycle(40);
	OC1CONbits.ON = 1;

	INTCONbits.MVEC = 1; // Enable multi interrupts
	__builtin_enable_interrupts();

	TRISDbits.TRISD8 = 1;
	unsigned char count = 40;
	while(1)
	{
		SetDutyCycle(count);
		if (PORTDbits.RD8 == 0)
		{
			count += 1;
			while (PORTDbits.RD8 == 0);
		}
	}
}