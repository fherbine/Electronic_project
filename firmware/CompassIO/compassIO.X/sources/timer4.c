/*
 * File:   timer4.c
 * Author: bocal
 *
 * Created on July 27, 2018, 11:34 AM
 */

#include "types.h"

void set_timer4(u8 timerms)
{
	PR4 = PBCLK/256/timerms;
	TMR4 = 0;
}

void handleMaxMinDegrees(s16 *degrees)
{
  if (*degrees < -90 || *degrees > 90)
    *degrees = (*degrees > 90) ? 0 : 180;
  else
    *degrees = 90 - *degrees;
}

void __ISR(_TIMER_4_VECTOR, IPL6SRS) Timer4Handler(void) {
    IFS0bits.T4IF = 0;
    if (devicePowered) {
	readMag(&mag_x, &mag_y);
	calibrateMag(mag_x, mag_y);
	s16 degrees = (int)readHeading(mag_x, mag_y);
        ft_putstr("[");
        ft_putnbr_base(degrees, 10);
        ft_putstr("]\n\r");
        ft_putnbr_base(mag_x, 10);
        ft_putstr("-");
        ft_putnbr_base(mag_y, 10);
        handleMaxMinDegrees(&degrees);
        ServoMotorSetAngle(degrees);
    }
}

void Init_Timer4()
{
  T4CON = 0;               // 0 on every bit, (timer stop, basic config)
  TMR4 = 0;                // Clean the timer register

  IPC4bits.T4IP = 6; // Set priority
  IPC4bits.T4IS = 1; // Set subpriority
  IFS0bits.T4IF = 0;       // Clear interrupt status flag
  IEC0bits.T4IE = 1; // Enable interrupts

  T4CONbits.TCKPS = 0b111; // Set scaler 1:256
  set_timer4(TIMER4_500MS);
  PR4 = PBCLK/256/2;    // Setup the period on 500ms
  T4CONbits.ON = 1;
}
