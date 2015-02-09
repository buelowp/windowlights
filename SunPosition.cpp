/*
    This file is part of WindowLights.

    SunPosition is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SunPosition is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
    
    This implementation is taken and modified from Mike Chirico at
    http://souptonuts.sourceforge.net/code/sunrise.c.html
    
    Modifications done have been in the spirit of the GPL, and
    any copying or reuse of this header and class are subject to
    the GPL v2 or any later GPL at the users discretion.
    All thanks for this work go to Mike who made it pretty easy.
*/
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
