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
    u8 output;

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

u8	store_several_datas(u32 addr, s32 *datas, u32 each_size, u8 n_data)
{
	u8 i = 0;
	s32 buffer = 0;



	while (i < n_data)
	{
		buffer |= (datas[i] << (each_size * 8 * i));
		i++;
		/*	in case of overflow */
		if ((buffer & (0x1 << 31)) && !(i < n_data))
			return (FALSE);
	}
	write_data(addr, buffer, each_size * n_data);
	return (TRUE);
}

s32		read_index_data(u32 addr, u32 size, u8 index)
{
	s32 mask = 0x0;

	if (size == 1)
		mask = 0xff;
	else if (size == 2)
		mask = 0xffff;
	else if (size == 3)
		mask = 0xffffff;
	else if (size == 4)
		mask = 0xffffffff;
	return ((read_data(addr, size) << index * size * 8) & mask); // get data at index
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

void init_struct_datas(struct s_data *data)
{
	data->dest_coord.lat = 0.0;
	data->dest_coord.lon = 0.0;
	ft_putfloat(data->dest_coord.lat);
	ft_putendl("");
	ft_putfloat(data->dest_coord.lon);
	ft_putendl("");
	data->dest_coord.lat = (((float)((u32)read_data(STORE_DEST_LAT_X1000, 4))) / 1000);
	//data->dest_coord.lat = 1.1;													///               TEST STUFF
	//data->dest_coord.lon = 1.1;													///				TEST STUFF
	delayms(85);
	data->dest_coord.lon = (((float)((u32)read_data(STORE_DEST_LONG_X1000, 4))) / 1000);
	delayms(85);
	ft_putfloat(data->dest_coord.lat);
	ft_putendl("");
	ft_putfloat(data->dest_coord.lon);
	ft_putendl("");
	if (data->dest_coord.lat != 0.0 && data->dest_coord.lon != 0.0) {
		data->dest_coord.completed = TRUE;
	}
	else
		data->dest_coord.completed = FALSE;
	data->init_coord.completed = FALSE;
	data->current_coord.completed = FALSE;
	data->current_coord.lat = 0.0;
	data->current_coord.lon = 0.0;
	data->init_coord.lat = 0.0;
	data->init_coord.lon = 0.0;
	data->init_distance = 0xFFFFFFFF;
	data->current_distance = 0xFFFFFFFF;
	data->store_data = FALSE;
}
