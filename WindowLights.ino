#include <Adafruit_GPS.h>
#include <FastLED.h>
#include <mk20dx128.h>
#include <pixelvector.h>

#include "Christmas.h"
#include "Halloween.h"
#include "Valentines.h"
#include "IndependenceDay.h"
#include "SunPosition.h"

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

bool runAnyway;
CRGB strip[NUM_STRIPS][NUM_LEDS];

void setup()
{
  runAnyway = false;
  pinMode(SWITCH_PIN, INPUT);
  attachInterrupt(SWITCH_PIN, isrService, FALLING);
  
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN2>(strip[1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN3>(strip[2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, PIN4>(strip[3], NUM_LEDS);
  randomSeed(analogRead(0));
}

void isrService()
{
  __disable_irq();
  runAnyway = true;
  __enable_irq();
}

void pixelShutdown()
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      strip[i][j] = CRGB::Black;
    }
  }
  FastLED.show();
}

void runChristmas()
{
  Winker wink(TOTAL_PIXELS, NUM_ACTIVE);
  int buff = 0;
  
  wink.startup();
  wink.setFistActive();
  wink.seeTheRainbow();
  
  while (whatTimeIsIt() == CHRISTMAS) {
    if (buff++ == 20) {
      buff = 0;
      wink.addOne();
    }
    wink.action();
    delay(25);
  }
  pixelShutdown();
}

void runValentines()
{
  Valentines vday(TOTAL_PIXELS);
  vday.startup();
  
  while (whatTimeIsIt() == VALENTINES) {
    vday.action();
    delay(500);
  }
  pixelShutdown();
}

void runIndependence()
{
  Independence iday(TOTAL_PIXELS);
  iday.startup();
  
  while (whatTimeIsit() == INDEPENDENCE) {
    iday.action();
    delay(500);
  }
  pixelShutdown();
}

void runHalloween()
{
  Halloween hday(TOTAL_PIXELS);
  hday.startup;
  
  while (whatTimeIsIt() == HALLOWEEN) {
    hday.action();
    delay(500);
  }
  pixelShutdown();
}

void loop()
{
  switch (whatTimeIsIt()) {
    case CHRISTMAS:
      runChristmas();
      break;
    case VALENTINES:
      runValentines();
      break;
    case MEMORIAL:
    case INDEPENDENCE:
      runIndependence();
      break;
    case HALLOWEEN:
      runHalloween();
      break;
    case THANKSGIVING:
    default:
      delay(60000);
  }
}
