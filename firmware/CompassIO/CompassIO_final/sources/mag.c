#include "types.h"

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
    //delayms(100);
    u8 l = MAG_Read(addr2);
    return ((h << 8) | l);
}

void readMag(s16 *x, s16 *y, s16 *z)
{
	*x = MagGetFullNumber(MAG_X_H, MAG_X_L);
	*y = MagGetFullNumber(MAG_Y_H, MAG_Y_L);
	*z = MagGetFullNumber(MAG_Z_H, MAG_Z_L);
}