#import "NeoRainbowEfx.h"

NeoRainbowEfx::NeoRainbowEfx(NeoStrip *strip, int startPixel, int len)
  : NeoWindow(strip, startPixel, len)
{
  
}

void NeoRainbowEfx:: setRainbowEfx(uint16_t waitTime)
{
  efxDone = false;
  effectDelay = waitTime;
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoRainbowEfx::rainbowEfxUpdate;
  
  curColor = 0;

}

void NeoRainbowEfx::rainbowEfxUpdate(void)
{
  for (int i=myStartPixel; i <= myEndPixel; i++)
  {
      myStrip->setPixelColor(i, NeoStrip::colorWheel((i+curColor) & 255));
  }

  curColor++;
  if (curColor > 255)
  {
    efxDone = true;
    curColor = 0;
  }
}

