#ifndef __LIGHTSTYPES_H__
#define __LIGHTSTYPES_H__

#include <vector>
#include "FastLED-Sparkcore/firmware/FastLED.h"

#define SWITCH_PIN     2
#define LED_STRIP_1    3
#define LED_STRIP_2    4
#define LED_STRIP_3    5
#define LED_STRIP_4    6
#define GPS_RX_PIN     7

#define LEDS_PER_STRIP  120
#define NUM_STRIPS      1
#define NUM_LEDS        (LEDS_PER_STRIP * NUM_STRIPS)
#define NUM_ACTIVE      20
#define NORMAL_BRIGHT   100
#define NUM_COLORS      6
#define TOTAL_PIXELS    (NUM_LEDS * NUM_STRIPS)
#define NON_PIXEL       -1

#define CHRISTMAS      100
#define HALLOWEEN      101
#define THANKSGIVING   102
#define VALENTINES     103
#define INDEPENDENCE   105
#define NORAH_BDAY		106
#define MADDIE_BDAY		107
#define METEOR_SHOWER	108
#define NEWYEARS		109
#define NO_PROGRAM      999

#define CST_OFFSET      -6
#define DST_OFFSET      (CST_OFFSET + 1)
#define TIME_BASE_YEAR      2015

#define LATITUDE        42.058102
#define LONGITUDE       87.984189

using namespace NSFastLED;

extern CRGB strip[NUM_STRIPS][NUM_LEDS];

void isrService();

#endif
