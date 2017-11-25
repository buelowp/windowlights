/*
 * MeteorShower.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: pete
 */

#include "MeteorShower.h"

MeteorShower::MeteorShower()
{
}

MeteorShower::~MeteorShower()
{
}

byte MeteorShower::adjacentCCW(byte i)
{
	byte r;

	if (i > 0) {
		r = i - 1;
	}
	else {
		r = LEDS_PER_STRIP - 1;
	}

	return r;
}

void MeteorShower::action()
{
	  //hue master
	  hue++;

	  //populate the leds[] with stored ledsX[] array data
	  for(byte i = 0; i < LEDS_PER_STRIP; i++ ) {
	    ledsX[i][0] = leds[i].r;
	    ledsX[i][1] = leds[i].g;
	    ledsX[i][2] = leds[i].b;
	  }
	  //clear the previous counter clockwise position
	  byte iCCW;

	  //we are keeping track of elapsed time
	  timeframe++;    //fx timer

	  //meteorLENGTH fx is only shown for this time frame
	  if((timeframe >= 1) && (timeframe <= 280)) { meteorLENGTH = 29; }
	  if((timeframe > 280) && (timeframe <= 500)) { meteorLENGTH = 45; }

	  //RAINBOWs fx add rainbow tails during this time frame only
	  if((timeframe > 0) && (timeframe <= 280)) { RAINBOWs = true; }
	  else{ RAINBOWs = false; }
	  //keep our RAINBOWs within a specific range of hue
	  if(RAINBOWs == true){ hue = hue - 20; if(hue <= 0){  hue = 1; } }

	  //RANDOMpick fx is only enabled during this timeframe
	  if((timeframe > 600) && (timeframe <= 790)) { RANDOMpick = true; }
	  else{ RANDOMpick = false; }

	  //pick a random spot in the meteor switch case statement below
	  if (RANDOMpick == true){ idex = random8(46); }
	  else{
	    //increment the meteor display frame
	    idex++;
	    //make sure we don't drift into space
	    if (idex > meteorLENGTH) { idex = 0; } }

	  //meteorLENGTH is randomized during this timeframe only
	  if((timeframe > 790) && (timeframe <= 1090)) { meteorLENGTH = random8(7, 38); }

	  //during this point in the animation timeframe
	  if(timeframe == 1180) {
	    //reset the timeframe
	    timeframe = 0;
	    //increment the loop counter
	    loopCount++;
	  }

	  //during this part of the loopCount, all meteors have a white colored tip
	  if(loopCount == 1) { colorTIP = 0; }
	  if(loopCount == 2) { colorTIP = 1; }
	  if(loopCount == 3) { colorTIP = random8(11); }
	  //end of the desired fx, reset the variable for the next time around
	  if(loopCount == 4) {
	    colorTIP = 0;
	    loopCount = 0;
	  }

	  //there are two switch case statements nestled into one another
	  //we always want to control the color of the meteor tip
	  //the other controls the actual meteor animation in 45 frames/case statements
	  switch (idex) {
	  case 0:
	    switch (colorTIP){
	    case 0:
	      leds[0] = CHSV(hue, 255, 255);
	      break;
	    case 1:
	      leds[0] = CRGB(100,100,100);
	      break;
	    case 2:
	      leds[0] = CRGB::Yellow;
	      break;
	    case 3:
	      leds[0] = CRGB::Violet;
	      break;
	    case 4:
	      leds[0] = CRGB::Green;
	      break;
	    case 5:
	      leds[0] = CRGB::Purple;
	      break;
	    case 6:
	      leds[0] = CRGB::Orange;
	      break;
	    case 7:
	      leds[0] = CRGB::Cyan;
	      break;
	    case 8:
	      leds[0] = CRGB::GreenYellow;
	      break;
	    case 9:
	      leds[0] = CRGB::Magenta;
	      break;
	    case 10:
	      leds[0] = CRGB::SkyBlue;
	    }
	    break;
	  case 1:
	    leds[0] = CHSV((hue - 20), 255, 210);
	    break;
	  case 2:
	    leds[0] = CHSV((hue - 22), 255, 180);
	    break;
	  case 3:
	    leds[0] = CHSV((hue - 23), 255, 150);
	    break;
	  case 4:
	    leds[0] = CHSV((hue - 24), 255, 110);
	    break;
	  case 5:
	    leds[0] = CHSV((hue - 25), 255, 90);
	    break;
	  case 6:
	    leds[0] = CHSV((hue - 26), 160, 60);
	    break;
	  case 7:
	    leds[0] = CHSV((hue - 27), 140, 40);
	    break;
	  case 8:
	    leds[0] = CHSV((hue - 28), 120, 20);
	    break;
	  case 9:
	    leds[0] = CHSV((hue - 29), 100, 20);
	    break;
	  case 10:
	  case 11:
	  case 12:
	  case 13:
	  case 14:
	  case 15:
	  case 16:
	  case 17:
	  case 18:
	  case 19:
	  case 20:
	  case 21:
	  case 22:
	  case 23:
	  case 24:
	  case 25:
	  case 26:
	  case 27:
	  case 28:
	  case 29:
	  case 30:
	  case 31:
	  case 32:
	  case 33:
	  case 34:
	  case 35:
	  case 36:
	  case 37:
	  case 38:
	  case 39:
	  case 40:
	  case 41:
	  case 42:
	  case 43:
	  case 44:
	  case 45:
	    leds[0] = CRGB::Black;
	    break;
	  }
	  //copy the LED Array
	  for(byte i = 1; i < LEDS_PER_STRIP; i++ ) {
	    iCCW = adjacentCCW(i);
	    leds[i].r = ledsX[iCCW][0];
	    leds[i].g = ledsX[iCCW][1];
	    leds[i].b = ledsX[iCCW][2];
	  }
}

void MeteorShower::seeTheRainbow()
{
	  //show the blinky
	  FastLED.show();
	  //control the animation speed/frame rate
	  delay(frameDelay);
}

void MeteorShower::startup()
{
}
