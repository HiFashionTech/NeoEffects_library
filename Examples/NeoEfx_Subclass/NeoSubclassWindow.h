// NeoSubclassWindow - a subclass of NeoWindow to add rainbow efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example
#include "NeoEffects.h"
#include "NeoWindow.h"

class NeoSubclassWindow : public NeoWindow 
{
  public:
    NeoSubclassWindow(NeoStrip *strip, int startPixel, int len);

  void setNewEffect(uint16_t wait, int type); // type 0 or 1; 1 evenly distributes color
private:
  void NewEffectUpdate(void);
  int curColor;
  int rainbowType;
};

