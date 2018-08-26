#include "types.h"

#define toRad(angle) ((angle) * M_PI / 180)
#define toDeg(angle) ((angle) / M_PI * 180)

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

  lat1 = toRad(lat1);
  lat2 = toRad(lat2);

  float y = sin(dLong) * cos(lat2);
  float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLong);
  float brng = toDeg(atan2(y, x));

  // fix negative degrees
  if(brng < 0) {
    brng = 360 - fabs(brng);
  }
  return brng;
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

