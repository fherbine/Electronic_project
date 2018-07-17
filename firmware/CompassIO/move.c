#include <stdio.h>
#include <math.h>

#define toRad(angle) ((angle) * M_PI / 180)
#define toDeg(angle) ((angle) / M_PI * 180)

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char *new_s1;
	unsigned char *new_s2;

	new_s1 = (unsigned char*)s1;
	new_s2 = (unsigned char*)s2;
	while ((*new_s1 || *new_s2) && n > 0)
	{
		if (*new_s1 != *new_s2)
			return (*new_s1 - *new_s2);
		new_s1++;
		new_s2++;
		n--;
	}
	return (0);
}

#define DISTANCE_KM 6371

double get_distance(double lat1, double long1, double lat2, double long2)
{
	return (
		2 * DISTANCE_KM * asin(
	    sqrt(
	      pow(sin(toRad(lat2 - lat1) / 2.0), 2.0) +
	      cos(toRad(lat1)) *
	      cos(toRad(lat2)) *
	      pow(sin(toRad(long2 - long1) / 2.0), 2.0)
	    )
	  )
	);
}

unsigned long int		ft_strlen(char *s)
{
	int count;

	count = 0;
	if (!s)
		return (0);
	while (s[count])
	{
		count++;
	}
	return (count);
}

#define NMEA_GPRMC_LATITUDE 2
#define NMEA_GPRMC_LONGITUDE 4

// NMEA
// Latitude -> DDmm.mmmm
// Longitude -> DDDmm.mmmm
void parse_nmea_gps(char *data)
{
  int size = ft_strlen(data);
	char separatorCount = 0;
	double lat = 0.0;
	double lon = 0.0;
	printf("%s\n", data);
  if (size > 6)
  {
    if (data[0] == '$' && data[1] == 'G' && data[2] == 'P' && data[3] == 'R' &&
    data[4] == 'M' && data[5] == 'C')
    {
      int i = 0;
			int degrees = 0;
			int min = 0;
			int sec = 0;
      while (i < size && data[i] != '\n')
      {
				if (data[i] == ',') {
					if (separatorCount == NMEA_GPRMC_LATITUDE) {
						if (i + 9 + 1 > size)
							return;
						if (data[i+1] == ',') {
							//printf("Empty value [1]\n");
							return; // Empty value
						} else {
							degrees = (data[i+1] - '0') * 10 + (data[i+2] - '0');
							min = (data[i+3] - '0') * 10 + (data[i+4] - '0');
							sec = ((data[i+6] - '0') * 1000 + (data[i+7] - '0') * 100 + (data[i+8] - '0') * 10 + (data[i+9] - '0')) * 60.0 / 10000.0; // Get secondes from xx.mmmm
							lat = (double)degrees + (double)min / 60.0 + (double)sec / 3600.0; // Degrees, Minutes and Secondes to Digital degrees
							i += 9;
						}
					} else if (separatorCount == NMEA_GPRMC_LONGITUDE) {
						if (i + 10 + 1 > size)
							return;
						if (data[i+1] == ',') {
							//printf("Empty value [2]\n");
							return; // Empty value
						} else {
							degrees = (data[i+1] - '0') * 100 + (data[i+2] - '0') * 10 + (data[i+3] - '0');
							min = (data[i+4] - '0') * 10 + (data[i+5] - '0');
							sec = ((data[i+7] - '0') * 1000 + (data[i+8] - '0') * 100 + (data[i+9] - '0') * 10 + (data[i+10] - '0')) * 60 / 10000; // Get secondes from xx.mmmm
							lon = (double)degrees + (double)min / 60.0 + (double)sec / 3600.0; // Degrees, Minutes and Secondes to Digital degrees
							i += 10;
						}
					}
					separatorCount++;
				}
        i++;
      }
    }
  }
	printf("Latitude: %f - Longitude: %f\n", lat, lon);
}

short get_direction(double lat1, double long1, double lat2, double long2)
{
  double dLat = toRad(lat2 - lat1);
  double dLong =  toRad(long2 - long1);

  lat1 = toRad(lat1);
  lat2 = toRad(lat2);

  double y = sin(dLong) * cos(lat2);
  double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLong);
  double brng = toDeg(atan2(y, x));

  // fix negative degrees
  if(brng < 0) {
    brng = 360 - fabs(brng);
  }

  return brng;
}

double parse_float(char *data)
{
	long long offset = 1;
	long long floatValue = 1;
	long long result = 0;
	short neg = 1;
	int i = ft_strlen(data);
	printf("%s|%d\n", data, i);
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
	printf("(double)(%lld / %lld) * %d\n", result, floatValue, neg);
	return (((double)result/(double)floatValue) * (double)neg);
}

// lat xx.xxxxxx\n
// long xxx.xxxxxx\n
double parser_gps_bluetooth(char *data)
{
	double output = 0.0;
	if (!ft_strncmp(data, "lat ", 4))
	{
		data += 4;
		output = parse_float(data);
		if (output < -90 || output > 90) {
			printf("Wrong latitude data");
		}
		// Store data in flash memory
	} else if (!ft_strncmp(data, "long ", 5))
	{
		data += 5;
		output = parse_float(data);
		if (output < -180 || output > 180) {
			printf("Wrong longitude data");
		}
		// Store data in flash memory
	}
	printf(">%f<\n", output);
	return (0.0);
}

int main()
{
  //printf("%f\n", get_distance(48.42133629395918, -4.553162271750807, 48.421042385350674, -4.553355834360332));
  //  parse_nmea_gps("$GPRMC,164933.270,A,4853.7671,N,00219.1216,E,1.74,74.12,010618,,,A*54\n"); //48.896118°N 2.318693°E https://rl.se/gprmc
  //	parse_nmea_gps("$GPRMC,114546.000,A,4853.8037,N,00219.0782,E,0.00,92.99,120718,,,A*5A\n");
  //	parse_nmea_gps("$GPRMC,115922.000,A,4853.7988,N,00219.1280,E,5.34,359.60,120718,,,A*61\n");
	double data = 123.456;
	printf("%f - %llx\n", data, data);
  // parser_gps_bluetooth("lat 85.999999");
  // parser_gps_bluetooth("lat 89.99999");
  
  return (0);
}

//   printf("%d\n", sizeof(double));
//
//
// //send
//   double lat;
//   for (char i = 0; i < 8; i++) {
//     sendchar(lat & 255)
//     lat >>= 8;
//   }
//   double lon;
//   for (char i = 0; i < 8; i++) {
//     sendchar(lon & 255)
//     lon >>= 8;
//   }
// //end send
//
// //receive
//   double lat = 0;
//   for (char i = 0; i < 8; i++) {
//     char tmp = readchar();
//     lat |= tmp;
//     lat <<= 8;
//   }
//   double lon = 0;
//   for (char i = 0; i < 8; i++) {
//     char tmp = readchar();
//     lon |= tmp;
//     lon <<= 8;
//   }
