// NeoRainbowEfx - a subclass of NeoWindow to add rainbow efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example

#include "NeoWindow.h"

class NeoRainbowEfx : public NeoWindow 
{
  public:
    NeoRainbowEfx(NeoStrip *strip, int startPixel, int len);

  void setRainbowEfx(uint16_t wait);
private:
  void rainbowEfxUpdate(void);
  int curColor;
};

