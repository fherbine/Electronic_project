#include "types.h"

void send_addr(u32 addr)
{
    u8 output;

    Handle_SPI((addr >> 16) & 0xFF, &output);
    Handle_SPI((addr >> 8) & 0xFF, &output);
    Handle_SPI(addr & 0xFF, &output);
}

void erase_sector(u32 addr)
{
    u8 output;

    _CS1_ON();
    Handle_SPI(FM_WRITE_ENABLE, &output);
    _CS1_OFF();
    delayms(100);
    _CS1_ON();
    Handle_SPI(FM_ERASE_SECTOR, &output);
    send_addr(addr);
    _CS1_OFF();
}

void erase_small_sector(u32 addr)
{
    u8 output;

    _CS1_ON();
    Handle_SPI(FM_WRITE_ENABLE, &output);
    _CS1_OFF();
    delayms(100);
    _CS1_ON();
    Handle_SPI(FM_ERASE_SMALL_SECTOR, &output);
    send_addr(addr);
    _CS1_OFF();
	delayms(50);
}

void write_data(u32 addr, s32 data, u32 size)
{
    u32 i = 0;
    long long output;

    erase_sector(addr);
    delayms(200);
    _CS1_ON();
    Handle_SPI(FM_WRITE_ENABLE, &output);
    _CS1_OFF();
    delayms(100);
    _CS1_ON();
    Handle_SPI(FM_PAGE_PROGRAM, &output);
    send_addr(addr);
    //output = *(long long *)&data;   // Float
    output = data;                    // Int
    while (i < size)
    {
        Handle_SPI((int)((long long)data >> (i * 8)) & 0xFF, &output);
        i++;
    }
    _CS1_OFF();
}

s32		read_data(u32 addr, u32 size)
{
    u32 i = 0;
    u8 output;
	s32	funcOut = 0;

    _CS1_ON();
    Handle_SPI(FM_READ, &output);
    send_addr(addr);
    while(i < size)
    {
        Handle_SPI(0x00, &output);
		funcOut = (int)((long long)output << (i * 8)) | funcOut;
        i++;
    }
    _CS1_OFF();
	return (funcOut);
}

void read_id()
{
    _CS1_ON();
    u8 output;
    Handle_SPI(FM_ID, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
    Handle_SPI(0, &output);
    ft_putnbr_base(output, 16);
    _CS1_OFF();
}

void read_status_register()
{
    _CS1_ON();
    u8 output;
    Handle_SPI(FM_WRITE_ENABLE, &output);
    _CS1_OFF();
    delayms(100);
    _CS1_ON();
    Handle_SPI(FM_STATUS_REGISTER_READ, &output);
   // Handle_SPI(0x0, &output);
    ft_putnbr_base(output, 16);
    Handle_SPI(FM_WRITE_DISABLE, &output);
    _CS1_OFF();
    delayms(100);
    _CS1_ON();
    Handle_SPI(FM_STATUS_REGISTER_READ, &output);
    Handle_SPI(0x0, &output);
    ft_putnbr_base(output, 16);
    _CS1_OFF();
}

/*
 *
 * double :
 *      gps init (lat + long) >> 2f, gps dest >> 2f
 *
 * int :
 *  distance init (pos init > dest) >> 1i
 *
 *
 */

void store_int(u32 addr, u32 data)
{
    _CS1_OFF();
    erase_sector(addr);
    delayms(200);
    write_data(addr, data, 4);
    delayms(85);
}
