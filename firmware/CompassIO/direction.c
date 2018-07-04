#include <stdio.h>
#include <math.h>

#define toRad(angle) ((angle) * M_PI / 180)
#define toDeg(angle) ((angle) / M_PI * 180)

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

main()
{
  printf("%d\n", get_direction(48.89666249816737, 2.3183298110961914, 50.040144614786946, 2.341561138008501));
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
}
