CRGB ColorWheel[] = {
  CRGB::Red,
  CRGB::OrangeRed,
  CRGB::Purple,
  CRGB::Green,
  CRGB::Blue,
};

class Halloween {
public:
  Halloween(int);
  ~Haloween();
  
  void seeTheRainbow();
  void startup();
  void action();
  
private:
  Pixels<CRGB> *pixels;
  int which;
  int totalPixels;
};

Halloween::Halloween(int p)
{
  pixels = new Pixels<CRGB>(p);
  totalPixels = p;
  which = 0;
}

Halloween::~Halloween()
{
  delete pixels;
}

void Halloween::startup()
{
  int window = 1;
  CRGB c;
  
  for (int i = 0; i < totalPixels; i++) {
    if (i % 115) {
      c = ColorWheel[window++];
    }
    pixels->push-back(c);
  }
  seeTheRainbow();          
}

void Halloween::action()
{
}

void Halloween::seeTheRainbow()
{
}

