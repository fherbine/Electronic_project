/*
 * File:   timer4.c
 * Author: bocal
 *
 * Created on July 27, 2018, 11:34 AM
 */

#include "types.h"

#define TIMER4_100MS 10
#define TIMER4_500MS 2

void set_timer4(u8 timerms)
{
	PR4 = PBCLK/256/timerms;
	TMR4 = 0;
}

s16 mag_x = 0.0;
s16 mag_y = 0.0;

void __ISR(_TIMER_4_VECTOR, IPL6) Timer4Handler(void) {
    IFS0bits.T4IF = 0;
	if (devicePowered) {
		readMag(&mag_x, &mag_y);
		calibrateMag(mag_x, mag_y);
		s16 degrees = (int)readHeading(mag_x - offset_x, mag_y - offset_y);
		if (degrees < -90 || degrees > 90)
			degrees = (degrees > 90) ? 0 : 180;
		else
			degrees = 90 - degrees;
		ft_putnbr_base(degrees, 10);
		ft_putstr("#");
		ServoMotorSetAngle(degrees);
		ft_putnbr_base(mag_x - offset_x, 10);
		ft_putstr(" ");
		ft_putnbr_base(mag_y - offset_y, 10);
		ft_putstr("\n\r");
	}
	//ft_putstr("This is timer4 !\n\r");
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
