// NeoRainbowEfx - a subclass of NeoWindow to add rainbow efx
// effect is taken from the Adafruit_NeoPixel Library's strandtest example
// type 0 = window shows myPixelCount portion of wheel each update, thru all 255 colors
// type 1 = evenly distributes wheel across N pixels in window (full rainbow in window)

#include "NeoRainbowEfx.h"

NeoRainbowEfx::NeoRainbowEfx(NeoStrip *strip, int startPixel, int len)
  : NeoWindow(strip, startPixel, len)
{
  
}

void NeoRainbowEfx:: setRainbowEfx(uint16_t waitTime, int type)
{
  // members common to all effects
  setNoEfx();// reset counters
  effectDelay = waitTime;
  // effect specfic members
  curColor = 0;
  rainbowType = type;

  // and set the update function - magic of function poiners
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoRainbowEfx::rainbowEfxUpdate;
}

void NeoRainbowEfx::rainbowEfxUpdate(void)
{
  // put a color in each pixel of window, depending on type
  for (int i=0; i <= myPixelCount; i++)
  {
    // colorWheel has 255 colors, from Adafruit NeoPixel strandtest example
    if (rainbowType == 0) {
      myStrip->setPixelColor(i+myStartPixel, NeoStrip::colorWheel((i+curColor) & 255));
    } else {
      myStrip->setPixelColor(i+myStartPixel, NeoStrip::colorWheel(((i*256/myPixelCount)+curColor) & 255));
    }
  }

  curColor++;
  if (curColor > 255)
  {
    // reached end of colors, mark effect done
    effectCount++;
    efxDone = true;
    // and set color index back to start so we can keep cycling
    curColor = 0;
  }
}

