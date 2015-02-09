#ifndef __SUNPOSITION_H__
#define __SUNPOSITION_H__

class SunPosition {
public:
  SunPosition();
  SunPosition(double, double, int);
  ~SunPosition();
  
  void setPosition(double, double, int);
  bool isSunset();
  bool isSunrise();
  void setTZOffset(int);
  
private:
  double degToRad(double);
  double radToDeg(double);
  double calcMeanObliquityOfEcliptic(double);
  double calcGeomMeanLongSun(double);
  double calcObliquityCorrection(double);
  double calcEccentricityEarthOrbit(double);
  double calcGeomMeanAnomalySun(double);
  double calcEquationOfTime(double);
  double calcTimeJulianCent(double);
  double calcSunTrueLong(double);
  double calcSunApparentLong(double);
  double calcSunDeclination(double);
  double calcHourAngleSunrise(double, double);
  double calcHourAngleSunset(double, double);
  double calcJD(int,int,int);
  double calcJDFromJulianCent(double);
  double calcSunEqOfCenter(double);
  double calcSunriseUTC(double, double, double);
  double calcSunsetUTC(double, double, double);
  
  double latitude;
  double longitude;
  int tzOffset;
};

#endif

