/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 29, 2018, 4:05 PM
 */

#include <p32xxxx.h>

#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = HS

#define SYSCLK (8000000/2*20/1) // 80Mhz
#define PBCLK (SYSCLK/1600)

/*
 * 
 */
int main(int argc, char** argv)
{
	T3CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR3 = 2;                // Clean the timer register
	IFS0bits.T3IF = 0;       // Clear interrupt status flag
	T3CONbits.TCKPS = 0b111; // Set scaler 1:256
	PR3 = PBCLK;    // Setup the period
	T3CONbits.ON = 1;

	OC1CONbits.OCM = 0b111;
	OC1RS = (PBCLK*1/20)*1.5;
	OC1R = (PBCLK*1/20)*1.5;
}
