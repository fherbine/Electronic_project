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

char	*ft_strchr(const char *s, int c)
{
	while (*s != (char)c && *s != '\0')
		s++;
	if (*s == (char)c)
		return ((char*)s);
	return (NULL);
}

int	ft_index(const char *s, int c)
{
	char *ptr;

	ptr = ft_strchr(s, c);
	if (ptr == NULL)
		return (-1);
	else
		return (ptr - s);
}

// lat xx.xxxxxx;long xxx.xxxxxx\n
double parser_gps_bluetooth(char *data)
{
	double lat = 0.0;
	double lon = 0.0;
	int separatorIndex = ft_index(data, ';');
	if (separatorIndex >= 0) {
		if (!ft_strncmp(data, "lat ", 4))
		{
			data += 4;
			lat = parse_float(data, separatorIndex - 4);
			if (lat < -90 || lat > 90) {
				ft_putendl("Wrong latitude data");
			}
			// Store data in flash memory
			data += separatorIndex - 4 + 1; // - "lat " (4) + ";" (1)
			if (!ft_strncmp(data, "long ", 5))
			{
				data += 5;
				lon = parse_float(data, ft_strlen(data));
				if (lon < -180 || lon > 180) {
					ft_putendl("Wrong longitude data");
				}
				// Store data in flash memory
			}
		} else {

		}
	} else {
            return (-1);
	}
        ft_putfloat(lat);
        ft_putstr(" ");
        ft_putfloat(lon);
	return (0.0);
}