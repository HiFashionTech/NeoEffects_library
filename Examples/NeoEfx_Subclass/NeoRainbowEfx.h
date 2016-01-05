// NeoRainbowEfx - a subclass of NeoWindow to add rainbow efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example
#include "NeoEffects.h"
#include "NeoWindow.h"

class NeoRainbowEfx : public NeoWindow 
{
  public:
    NeoRainbowEfx(NeoStrip *strip, int startPixel, int len);

  void setRainbowEfx(uint16_t wait, int type); // type 0 or 1; 1 evenly distributes color
  void setRainbowCycleEfx(uint16_t wait);
private:
  void rainbowEfxUpdate(void);
  void rainbowCycleEfxUpdate(void);
  int curColor;
  int rainbowType;
};

