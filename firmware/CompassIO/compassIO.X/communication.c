/*
 * File:   communication.c
 * Author: vomnes
 *
 * Created on July 10, 2018, 10:27 PM
 */

#include "types.h"

double parse_float(char *data)
{
	long long offset = 1;
	long long floatValue = 1;
	long long result = 0;
	short neg = 1;
	int i = ft_strlen(data);
	while (i)
	{
		UART2_Send_String(data[i]);
		if (i - 1 == 0 && data[0] == '-')
		{
			neg = -1;
			break;
		}
		if (data[i - 1] != '.' && data[i - 1] != '-' && data[i - 1] >= '0' && data[i - 1] <= '9')
		{
			result += (data[i - 1] - '0') * offset;
			offset *= 10;
		} else if (data[i - 1] == '.')
			floatValue = offset;
		else if (data[i - 1] < '0' || data[i - 1] > '9') // Handle wrong format
			return (999.999);
		i--;
	}
	return (((double)result/(double)floatValue) * (double)neg);
}

// lat xx.xxxxxx\n
// long xxx.xxxxxx\n
void parser_gps_bluetooth(char *data)
{
	double output = 0.0;
	ft_putendl("Analysing !");
	if (!ft_strncmp(data, "lat ", 4))
	{
		data += 4;
		output = parse_float(data);
		if (output < -90 || output > 90) {
			ft_putendl("Wrong latitude data");
		} else {
			ft_putendl("Latitude received");
		}
		// Store data in flash memory
	} else if (!ft_strncmp(data, "long ", 5))
	{
		data += 5;
		output = parse_float(data);
		if (output < -180 || output > 180) {
			ft_putendl("Wrong longitude data");
		} else {
			ft_putendl("Longitude received");
		}
		// Store data in flash memory
	}
	ft_putendl("Done");
}
