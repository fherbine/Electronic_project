/*
 * File:   communication.c
 * Author: vomnes
 *
 * Created on July 10, 2018, 10:27 PM
 */

#include "types.h"

double parse_float(char *data, unsigned int size)
{
	long long offset = 1;
	long long floatValue = 1;
	long long result = 0;
	short neg = 1;
	int i = size;
	while (i)
	{
		if (i - 1 == 0 && data[0] == '-')
		{
			neg = -1;
			break;
		}
		if (data[i - 1] != '.' && data[i - 1] != '-')
		{
			result += (data[i - 1] - '0') * offset;
			offset *= 10;
		} else if (data[i - 1] == '.')
			floatValue = offset;
		else if (data[i - 1] < '0' && data[i - 1] > '9') // Handle wrong format
			return (999.999);
		i--;
	}
	return (((double)result/(double)floatValue) * (double)neg);
}

// lat xx.xxxxxx;long xxx.xxxxxx
double parser_gps_bluetooth(char *data, s_data *data)
{
	double lat = 0.0;
	double lon = 0.0;
	int separatorIndex = ft_index(data, ';');
	if (separatorIndex >= 0) {
		if (!ft_strncmp(data, "lat ", 4))
		{
			data += 4;
			lat = parse_float(data, separatorIndex - 4);
			if (lat < -90.0 || lat > 90.0) {
				ft_putendl("Wrong latitude data");
				data->dest_coord.completed = FALSE;
				return (-1);
			}
			data += separatorIndex - 4 + 1; // - "lat " (4) + ";" (1)
			if (!ft_strncmp(data, "long ", 5))
			{
				data += 5;
				lon = parse_float(data, ft_strlen(data));
				if (lon < -180.0 || lon > 180.0) {
					ft_putendl("Wrong longitude data");
					data->dest_coord.completed = FALSE;
					return (-1);
				}
			} else {
				data->dest_coord.completed = FALSE;
				return (-1);
			}
		} else {
			data->coord.completed = FALSE;
			return (-1);
		}
	} else {
		ft_putendl("Invalid data");
		data->dest_coord.completed = FALSE;
		return (-1);
	}
	data->dest_coord.completed = TRUE;
	data->dest_coord.lat = lat;
	data->dest_coord.lon = lon;
	// Store data in flash memory
	erase_sector(STORE_DEST_LAT_X1000);
	delayms(85);
	write_data(STORE_DEST_LAT_X1000, lat * 1000, 4);
	delayms(85);
	erase_sector(STORE_DEST_LONG_X1000);
	delayms(85);
	write_data(STORE_DEST_LONG_X1000, lon * 1000, 4);
	delayms(85);
	return (0.0);
}
