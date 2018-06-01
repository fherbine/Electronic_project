/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 31, 2018, 2:34 PM
 */

#include "types.h"

#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = HS

/*
 * RD4 >> nRST
 * RD5 >> ON_OFF
 */

unsigned char  rst = 0, tr0 = 0, on_off = 0, gps = 0;
unsigned short tmp = 0, tmp2 = 0;

void	gps_power_on(void)
{
	tmp++;
	if (tmp == 100 && !rst) // 100ms after PIC power on
	{
		LATDbits.LATD4 = 1;
		rst = 1;
	}
	else if (tmp == 110 && !on_off)
	{
		LATDbits.LATD5 = 1;
		on_off = 1;
	}
	else if (tmp == 310 && on_off)
	{
		LATDbits.LATD5 = 0;
		on_off = 0;
	}
	else if (tmp > 310)
	{
		tmp = 0;
		gps = 1;
	}
}

void	gps_power_off(void)
{
	if (tmp == 0)
		LATDbits.LATD5 = 1;
	else if (tmp == 200)
		LATDbits.LATD5 = 0;
	tmp++;
}

void __ISR(_TIMER_2_VECTOR, IPL1) Timer2Handler(void) {
	TMR2 = 0;
	if (!gps)
		gps_power_on();
	else if ((PORTDbits.RD8 == 0 && gps) || gps == 2)
	{
		gps = 2;
		gps_power_off();
	}
	//LATDbits.LATD4 ^= 1;
	IFS0bits.T2IF = 0;
}

int main()
{
	__builtin_disable_interrupts();
	TRISDbits.TRISD4 = 0;				// RD4 is an output
	LATDbits.LATD4 = 0;					// RD4 is low by default

	TRISDbits.TRISD5 = 0;				// RD5 is an output
	LATDbits.LATD5 = 0;					// RD5 is low by default

	TRISDbits.TRISD8 = 1;

	T2CON = 0;               // 0 on every bit, (timer stop, basic config)
	TMR2 = 0;                // Clean the timer register
	IFS0bits.T2IF = 0;     // Clear interrupt status flag
	T2CONbits.TCKPS = 0b110; // Set scaler 1:256
	PR2 = PBCLK/64/1000;    // Setup the period > Periph. Bus Clk (set by user) / scaler > sec | 20ms

    IPC2bits.T2IP = 1; // Set priority
    IPC2bits.T2IS = 2; // Set subpriority
    IFS0bits.T2IF = 0; // Clear interrupt status flag
    IEC0bits.T2IE = 1; // Enable interrupts
	T2CONbits.ON = 1;


	INTCONbits.MVEC = 1; // Enable multi interrupts
	UART2_Init(0, 1, 0b11);
	__builtin_enable_interrupts();

	while(1)
	{
		UART2_Send_Data_Byte('t');
	}
}