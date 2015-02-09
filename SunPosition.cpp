#include "SunPosition.h"

SunPostion::SunPosition()
{
  latitude = 0.0;
  longitude = 0.0;
  tzData = 0;
}

SunPosition::SunPosition(double lat, double lon, int tz)
{
  latitude = lat;
  longitude = lon;
  tzData = tz;
}
