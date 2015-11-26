// NeoRainbowEfx - a subclass of NeoWindow to add rainbow efx
#include "NeoWindow.h"

class NeoRainbowEfx : public NeoWindow 
{
  public:
  void setRainbowEfx(uint16_t wait);
private:
  void rainbowEfxUpdate(void);
  int curColor;
};

