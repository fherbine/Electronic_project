/*
 * File:   gps.c
 * Author: vomnes
 *
 * Created on July 11, 2018, 10:24 AM
 */

#include "types.h"

#define NMEA_GPRMC_LATITUDE 2
#define NMEA_GPRMC_LONGITUDE 4

// NMEA
// Latitude -> DDmm.mmmm
// Longitude -> DDDmm.mmmm
int		parse_nmea_gps(char *data, struct s_data *data_s)
{
	int size = ft_strlen(data);
	char separatorCount = 0;
	double lat = 0.0;
	double lon = 0.0;
	if (size > 6)
	{
		if (!ft_strncmp(data, "$GPRMC,", 7))
		{
			int i = 0;
			int degrees = 0;
			int min = 0;
			int sec = 0;
			int neg = 1;
			while (i < size && data[i] != '\0')
			{
				if (data[i] == ',') {
					if (separatorCount == NMEA_GPRMC_LATITUDE) {
						if (i + 9 + 1 > size)
							return (0);
						if (data[i+1] == ',') {
							return (999); // Empty value
						} else {
							if (data[i + 1] == '-')
							{
								neg = -1;
								i++;
							}
							degrees = (data[i+1] - '0') * 10 + (data[i+2] - '0');
							min = (data[i+3] - '0') * 10 + (data[i+4] - '0');
							sec = ((data[i+6] - '0') * 1000 + (data[i+7] - '0') * 100 + (data[i+8] - '0') * 10 + (data[i+9] - '0')) * 60.0 / 10000.0; // Get secondes from xx.mmmm
							lat = neg * ((double)degrees + (double)min / 60.0 + (double)sec / 3600.0); // Degrees, Minutes and Secondes to Digital degrees
							i += 9;
						}
					} else if (separatorCount == NMEA_GPRMC_LONGITUDE) {
						if (i + 10 + 1 > size)
							return (0);
						if (data[i+1] == ',') {
							return (999); // Empty value
						} else {
							if (data[i + 1] == '-')
							{
								neg = -1;
								i++;
							}
							degrees = (data[i+1] - '0') * 100 + (data[i+2] - '0') * 10 + (data[i+3] - '0');
							min = (data[i+4] - '0') * 10 + (data[i+5] - '0');
							sec = ((data[i+7] - '0') * 1000 + (data[i+8] - '0') * 100 + (data[i+9] - '0') * 10 + (data[i+10] - '0')) * 60 / 10000; // Get secondes from xx.mmmm
							lon = neg * ((double)degrees + (double)min / 60.0 + (double)sec / 3600.0); // Degrees, Minutes and Secondes to Digital degrees
							i += 10;
						}
					}
					separatorCount++;
				}
				i++;
			}
      if (data_s->init_coord.completed == FALSE && data_s->dest_coord.completed ==  TRUE) {
        data_s->init_coord.completed = TRUE;
        data_s->init_coord.lat = lat;
        data_s->init_coord.lon = lon;
        data_s->init_distance = get_distance(data_s->init_coord.lat, data_s->init_coord.lon, data_s->dest_coord.lat, data_s->dest_coord.lon);
      }
      if (data_s->init_coord.completed == TRUE) {
        data_s->current_coord.lat = lat;
        data_s->current_coord.lon = lon;
        data_s->current_distance = get_distance(data_s->current_coord.lat, data_s->current_coord.lon, data_s->dest_coord.lat, data_s->dest_coord.lon);
      }
			return (1);
		}
	}
	return(0);
}
