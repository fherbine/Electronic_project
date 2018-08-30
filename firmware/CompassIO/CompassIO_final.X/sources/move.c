#include "types.h"

#define DISTANCE_KM 6371

float get_distance(float lat1, float long1, float lat2, float long2)
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

s16 get_direction(float lat1, float long1, float lat2, float long2)
{
  float dLat = toRad(lat2 - lat1);
  float dLong =  toRad(long2 - long1);
  float D = 0;

  lat1 = toRad(lat1);
  lat2 = toRad(lat2);
  long1 = toRad(long1);
  long2 = toRad(long2);

  //if (long1 > long2)
//	D = 6378.0 * acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(long1 - long2));
 // else
	//  D = 6378.0 * acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(long2 - long1));

  float T = cos(lat1 - lat2)*sin((M_PI - (2.0 * long1))/2.0)*sin((M_PI - (2.0 * long2))/2.0)*sin((M_PI - (2.0 * lat1))/2.0)*sin((M_PI - (2.0 * lat2))/2.0);
  float A =  -1.0*atan(T/sqrt(1.0 - T * T))+ (M_PI/2);
  float R = (sin(long1)-sin(long2)*cos(A))/sin(A)*cos(long2);
  float Arc = -1.0 * atan(R/sqrt(1-R*R))+ (M_PI/2);

  Arc = (long1 > long2) ? (Arc * 180)/M_PI : 360 - (Arc * 180)/M_PI;
  return (Arc);
}

void switch_position(struct s_data *datas)
{
	float tmp0 = 0.0, tmp1 = 0.0;
	tmp0 = datas->dest_coord.lat;
	tmp1 = datas->dest_coord.lon;
	datas->dest_coord.lat = datas->init_coord.lat;
	datas->dest_coord.lon = datas->init_coord.lon;
	datas->init_coord.lat = tmp0;
	datas->init_coord.lon = tmp1;
}

