// NeoRainbowEfx - a subclass of NeoWindow to add rainbow efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example

#include "NeoRainbowEfx.h"

NeoRainbowEfx::NeoRainbowEfx(NeoStrip *strip, int startPixel, int len)
  : NeoWindow(strip, startPixel, len)
{
  
}

void NeoRainbowEfx:: setRainbowEfx(uint16_t waitTime)
{
  // members common to all effects
  efxDone = false;
  effectDelay = waitTime;
  // effect specfic members
  curColor = 0;
  // and set the update function - magic of function poiners
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoRainbowEfx::rainbowEfxUpdate;
}

void NeoRainbowEfx::rainbowEfxUpdate(void)
{
  for (int i=myStartPixel; i <= myEndPixel; i++)
  {
      // colorWheel has 255 colors, from Adafruit NeoPixel strandtest example
      myStrip->setPixelColor(i, NeoStrip::colorWheel((i+curColor) & 255));
  }

  curColor++;
  if (curColor > 255)
  {
    // reached end of colors, mark effect done
    efxDone = true;
    // and set color index back to start so we can keep cycling
    curColor = 0;
  }
}

