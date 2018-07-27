/*
 * File:   timer3.c
 * Author: bocal
 *
 * Created on July 27, 2018, 11:40 AM
 */

#include "types.h"

void	gps_power_on(void)
{
	if (gpsTmp == 100 && !rst) // 100ms after PIC power on
	{
		LATDbits.LATD6 = 1;
		rst = 1;
	}
	else if (gpsTmp == 110 && !on_off)
	{
		LATDbits.LATD5 = 1;
		on_off = 1;
	}
	else if (gpsTmp == 310 && on_off)
	{
		LATDbits.LATD5 = 0;
		on_off = 0;
	}
	else if (gpsTmp > 310)
	{
		gpsTmp = 0;
		gps = 1;
        powerOnProcess = FALSE;
	}
}

void	gps_power_off(void)
{
	if (gpsTmp == 0)
		LATDbits.LATD5 = 1;
	else if (gpsTmp == 200)
        {
           	LATDbits.LATD5 = 0;
                powerOffProcess = FALSE;
        }
	gpsTmp++;
}

#define MAX_U16 0xFFFF

void __ISR(_TIMER_3_VECTOR, IPL1SRS) Timer3Handler(void) {
	IFS0bits.T3IF = 0;
    /* Power Off 2 Sec */
    if (countTimeEnable && countTime < MAX_U16)
        countTime++;
    /* ON OFF GPS Sequence */
    if (powerOnProcess)
        gps_power_on();
    if (powerOffProcess)
        gps_power_off();
	if (IsCalMode)
		TimerCalMode++;
    gpsTmp++;
}
