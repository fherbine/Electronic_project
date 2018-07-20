/*
 * File:   delay.c
 * Author: bocal
 *
 * Created on June 22, 2018, 10:05 AM
 */

#include "types.h"

void delayms(u32 ms) {
    while (ms)
	if (IFS0bits.T3IF == 1) {
	    IFS0bits.T3IF = 0;
	    ms--;
	}
}

void Init_Delay()
{
  T3CON = 0;               // 0 on every bit, (timer stop, basic config)
  TMR3 = 2;                // Clean the timer register

  IPC3bits.T3IP = 1; // Set priority
  IPC3bits.T3IS = 2; // Set subpriority
  IFS0bits.T3IF = 0;       // Clear interrupt status flag
  IEC0bits.T3IE = 1; // Enable interrupts

  T3CONbits.TCKPS = 0b111; // Set scaler 1:256
  PR3 = PBCLK/256/1000;    // Setup the period
  T3CONbits.ON = 1;
}
