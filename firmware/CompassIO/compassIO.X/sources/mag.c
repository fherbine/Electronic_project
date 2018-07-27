/*
 * File:   mag.c
 * Author: compassIO
 *
 * Created on July 23, 2018, 4:11 PM
 */

#include "types.h"

#define MAG_ADDR 0x0E

#define MAG_X_H 0x01
#define MAG_X_L 0x02
#define MAG_Y_H 0x03
#define MAG_Y_L 0x04
#define MAG_Z_H 0x05
#define MAG_Z_L 0x06
#define MAG_WHO_AM_I 0x07
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11

/* Calibrate MAG3110 */
s16	x_min = 0x7FFF;
s16	x_max = 0x8000;
s16	y_min = 0x7FFF;
s16	y_max = 0x8000;

s16 offset_x = 0;
s16 offset_y = 0;

float x_scale = 0;
float y_scale = 0;

s32 TimerCalMode = 0;
u8 IsCalMode = FALSE;

u8 MAG_Read(u8 addr)
{
    I2C1_Send_Data(addr, MAG_ADDR);
    return I2C1_Receive_Data(MAG_ADDR);
}

void init_mag()
{
	u8 whoiam = MAG_Read(MAG_WHO_AM_I);
	ft_putnbr_base(whoiam, 16);
	ft_putstr("\n\r");
	if (whoiam != 0xC4)
		ft_putendl("MAG3110 not connected");
	I2C1_Write_Data(MAG_ADDR, MAG_CTRL_REG2, 0x80); // Enable AutoMRST_EN bit
	delayms(15);
	I2C1_Write_Data(MAG_ADDR, MAG_CTRL_REG1, 1); // Enable MAG3110
}

s16 MagGetFullNumber(u8 addr1, u8 addr2)
{
    s16 h = MAG_Read(addr1);
    u8 l = MAG_Read(addr2);
    return ((h << 8) | l);
}

void readMag(s16 *x, s16 *y)
{
	*x = MagGetFullNumber(MAG_X_H, MAG_X_L);
	*y = MagGetFullNumber(MAG_Y_H, MAG_Y_L);
}

void calibrateMag(s16 x, s16 y)
{
	if (IsCalMode == TRUE)
	{
		if(x < x_min)
		  x_min = x;
		if(x > x_max)
		  x_max = x;
		if(y < y_min)
		  y_min = y;
		if(y > y_max)
		  y_max = y;
		if(TimerCalMode > 5000) // Exit calibration
		{
			IsCalMode = FALSE;
			LATFbits.LATF1 = 0;
			ft_putstr("x_min: ");
			ft_putnbr_base(x_min, 10);
			ft_putstr(" x_max: ");
			ft_putnbr_base(x_max, 10);
			ft_putstr(" y_min: ");
			ft_putnbr_base(y_min, 10);
			ft_putstr(" y_max: ");
			ft_putnbr_base(y_max, 10);
			ft_putstr("\n\r");
			offset_x = (x_min + x_max) / 2;
			offset_y = (y_min + y_max) / 2;
			ft_putstr(" offset x: ");
			ft_putnbr_base(offset_x, 10);
			ft_putstr(" offset y: ");
			ft_putnbr_base(offset_y, 10);
			ft_putstr("\n\r");
			x_scale = 1.0/(float)(x_max - x_min);
			y_scale = 1.0/(float)(y_max - y_min);
			ft_putstr(" x_scale: ");
			ft_putnbr_base(x_scale, 10);
			ft_putstr(" y_scale: ");
			ft_putnbr_base(y_scale, 10);
			ft_putstr("\n\r");
      set_timer4(TIMER4_500MS);
		}
	}
}

#define DEG_PER_RAD (180.0/3.14159265358979)

s16 readHeading(s16 x, s16 y)
{
	float xf = (float) x * 1.0f;
	float yf = (float) y * 1.0f;

	//Calculate the heading
	ft_putfloat(atan2(-yf*y_scale, xf*x_scale) * DEG_PER_RAD);
	ft_putstr("[]");
	return (atan2(-yf*y_scale, xf*x_scale) * DEG_PER_RAD);
}
