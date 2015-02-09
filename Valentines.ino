class Valentines {
public:
  Valentines(int);
  ~Valentines();
  
  void seeTheRainbow();
  void startup();
  void action();
  
private:
  Pixels<CRGB> *pixels;
  int totalPixels;
  int which;
};

Valentines::Valentines(int p)
{
  pixels = new Pixels<CRGB>(p);
  totalPixels = p;
  which = 0;
}

void Valentines::startup()
{
  for (int i = 0; i < totalPixels; i++) {
    if ((i % 2) == 0) {
      pixels->push_back(CRGB::Red);
    }
    else {
      pixels->push_back(CRGB::White);
    }
    seeTheRainbow();
    which = 1;
}

void Valentines::action()
{
  switch (which) {
    case 0:
      for (int i = 0; i < totalPixels; i++) {
        if ((i % 2) == 1) {
          pixels[i] = CRGB::Red;
        }
        else {
          pixels[i] = CRGB::White;
        }
      }
      which = 1;
      break;
    case 1:
      for (int i = 0; i < totalPixels; i++) {
        if ((i % 2) == 0) {
          pixels[i] = CRGB::Red;
        }
        else {
          pixels[i] = CRGB::White;
        }
      }
      which = 0;
      break;
  }
  seeTheRainbow();
}

void Valentines::seeTheRainbow()
{
  int index = -1;
  for (int i = 0; i < totalPixels; i++) {
    if ((i % NUM_LEDS) == 0)
      index++;
      
    strip[index][i % NUM_LEDS] = pixels[i];
  }
  FastLED.setBrightness(100);
  FastLED.show();
}

