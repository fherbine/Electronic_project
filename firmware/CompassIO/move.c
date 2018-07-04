#include <stdio.h>
#include <math.h>

#define toRad(angle) ((angle) * M_PI / 180)
#define toDeg(angle) ((angle) / M_PI * 180)

unsigned long int		ft_strlen(char *s)
{
	char *str = s;
	while (*s++);
	return (s-str);
}

void parse_nmea_gps(char *data)
{
  int size = ft_strlen(data);
  if (size > 6)
  {
    if (data[0] == '$' && data[1] == 'G' && data[2] == 'P' && data[3] == 'R' &&
    data[4] == 'M' && data[5] == 'C')
    {
      int i = 0;
      while (i < size && data[i] != '\n')
      {
        printf("%c", data[i]);
        i++;
      }
    }
  }
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

int main()
{
  //printf("%d\n", get_direction(48.89666249816737, 2.3183298110961914, 50.040144614786946, 2.341561138008501));
  parse_nmea_gps("$GPRMC,164933.270,A,4853.7671,N,00219.1216,E,1.74,74.12,010618,,,A*54\n");
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
