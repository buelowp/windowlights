HSVHue ColorWheel[] = {
  HUE_RED,
  HUE_YELLOW,
  HUE_BLUE,
  HUE_GREEN,
  HUE_PURPLE,
  HUE_ORANGE,
};

#define GOING_UP        0
#define GOING_DOWN      1
#define RETURN_TO_NORM  2

class Winker {
public:
  Winker(int, int);
  ~Winker();
  
  void startup();
  void action();
  void setFirstActive();
  void addOne();
  void seeTheRainbow();
  
private:
  Pixels<CHSV> *pixels;
  LedPixelMap *pixelMap;
  int totalPixels;
  int numActive;
  int index;
};

Winker::Winker(int p, int a)
{
  pixels = new Pixels<CHSV>(p);
  pixelMap = new LedPixelMap(a);
  totalPixels = p;
  numActive = a;
  int index = 0;
}

Winker::~Winker()
{
  delete pixels;
  delete pixelMap;
}

void Winker::Startup()
{
  for (int i = 0; i < totalPixels; i++) {
    CHSV c;
    c.h = ColorWheel[random(0, NUM_COLORS)];
    c.s = 255;
    c.v = NORMAL_BRIGHT;
    pixels->push_back(c);
  }
}

/**
 * Initially set 20 pixels active and getting brighter. This number of
 * active pixels might be dynamic, but should grow to be about 20 - 25% of
 * all the pixels
 */
void Winker::setFirstActive(int c)
{
  int count = 0;
  bool found = false;
  
  while (count < c) {
    int pixel = random(0, TOTAL_PIXELS);
    for (int j = 0; j < pixelMap.size(); j++) {
      if (pixelMap[j] == pixel)
        found = true;
        break;
    }
    if (!found) {
      pixelMap->pushBack(pixel, GOING_UP);
      found = false;
      count++;
    }
  }
}

void Winker::action()
{
  for (int i = 0; i < pixelMap->size(); i++) {
    if (pixelMap[i] == NON_PIXEL)
      continue;
      
    CHSV pixel = pixels[pixelMap[i]];
    
    switch (pixelMap->pixelDir(i)) {
    case GOING_UP:
      if (pixel.v == 254) {
        pixel.v = 255;
        pixelMap->setPixelDir(i, GOING_DOWN);
      }
      else {
        pixel.v += 2;
      }
      pixels[pixelMap[i]] = pixel;
      break;
    case GOING_DOWN:
      if (pixel.v == 1) {
        pixel.v = 0;
        pixel.h = ColorWheel[random(0, NUM_COLORS)];
        pixel.s = 255;
        pixelMap->setPixelDir(i, RETURN_TO_NORM);
      }
      else {
        pixel.v -= 2;
      }
      pixels[pixelMap[i]] = pixel;
      break;
    case RETURN_TO_NORM:
      if (pixel.v == (NORMAL_BRIGHT - 1)) {
        pixel.v = NORMAL_BRIGHT;
      }
      else {
        pixel.v += 2;
      }
      pixels[pixelMap[i]] = pixel;
      break;
    }    
  }
  seeTheRainbow();
}

void Winker::addOne()
{
  int pixel = random(0, TOTAL_PIXELS);
  bool added = false;
  
  for (int i = 0; i < pixelMap->size(); i++) {
    if (pixelMap[i] == pixel)
      return;
  }
  for (int i = 0; i < NUM_ACTIVE; i++) {
    if (pixelMap[i] == NON_PIXEL) {
      pixelMap->pushBack(pixel, GOING_UP);
      return;
    }
  }
  if (pixelMap->size() < NUM_ACTIVE) {
    pixelMap->pushBack(pixel, GOING_UP);
  }
}

CHSV Winker::getNextPixel()
{
  if (index == pixels->size()) {
    CHSV a;
    memset(a, 0, sizeof(CHSV));
    return a;
  }
  
  return pixels[index++];
}

void Winker::seeTheRainbox()
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    CHSV s[NUM_LEDS];
    for (int j = 0; j < NUM_LEDS; j++) {
      s[j] = pixels[(i + 1) * j];
    }
    hsv2rgb_rainbow(s, strip[i], NUM_LEDS);
  }
  FastLED.show();
}

