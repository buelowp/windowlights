/*
 * Maddie.h
 *
 *  Created on: Nov 9, 2015
 *      Author: pete
 */

#ifndef USER_APPLICATIONS_WINDOWLIGHTS_MADDIE_H_
#define USER_APPLICATIONS_WINDOWLIGHTS_MADDIE_H_

#include "WindowLights.h"

class Maddie {
public:
	Maddie(int);
	virtual ~Maddie();

	void seeTheRainbow();
	void startup();
	void action();

private:
	  std::vector<CRGB> pixels;
	  int totalPixels;
	  int which;
};

#endif /* USER_APPLICATIONS_WINDOWLIGHTS_MADDIE_H_ */
