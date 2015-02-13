#ifndef __LIGHTSTYPES_H__
#define __LIGHTSTYPES_H__

#define SWITCH_PIN     2
#define LED_STRIP_1    3
#define LED_STRIP_2    4
#define LED_STRIP_3    5
#define LED_STRIP_4    6
#define GPS_RX_PIN     7

#define NUM_LEDS        115
#define NUM_STRIPS      4
#define NUM_ACTIVE      60
#define NORMAL_BRIGHT   100
#define NUM_COLORS      6
#define TOTAL_PIXELS    (NUM_LEDS * NUM_STRIPS)
#define NON_PIXEL       -1

#define CHRISTMAS      100
#define HALLOWEEN      101
#define THANKSGIVING   102
#define VALENTINES     103
#define MEMORIAL       104
#define INDEPENDENCE   105
#define NOHOLIDAY      999

extern CRGB strip[NUM_STRIPS][NUM_LEDS];

#endif
