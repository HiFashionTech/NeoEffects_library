
// NeoStrip 
//   a class derived from Adafruit_NeoPixel
//   adds a simple boolean stripChanged
// This allows us to set/clear/test when we run a NeoWindow Effect
// and only run the show function if we changed the strip
// note that this requires all writes to use setStripChanged(),
// which may not be true, if the code directly accesses strip.setPixel()

#ifndef _NEOSTRIP_
#define _NEOSTRIP_

#include <Adafruit_NeoPixel.h>

class NeoStrip: public Adafruit_NeoPixel {
  private:
    boolean stripChanged;
    static int s_nIDGenerator;
    int myId;
    boolean *pixelActive; // array of booleans to define if pixel is in use
    
 public:

 NeoStrip(uint16_t n, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);

 void setStripChanged();
 void clearStripChanged();
 boolean getStripChanged();

 // some simple functions to effect whole strip
 void clearStrip();
 void fillStrip(uint32_t c);

 void printId();

 void show(void); // pass thru to parent, but clear stripChanged


//////
  void clearActive();
  void setPixelActive(int idx);
  void setPixelInactive(int idx);
  boolean isPixelActive(int idx);
  
 //////
 // helper function to create random Color (unit32_t of r, g,b)
 static uint32_t randomColor(void) { return Adafruit_NeoPixel::Color(random(0,255),random(0,255),random(0,255));}
 static uint32_t randomColor(uint32_t c1, uint32_t c2);
 static uint8_t getRed(uint32_t c) { return (uint8_t)(c >> 16);}
 static uint8_t getGreen(uint32_t c) { return (uint8_t)(c >>  8);}
 static uint8_t getBlue(uint32_t c) { return (uint8_t)c;}

 static const uint32_t White;
 static uint32_t colorWheel(byte WheelPos);
 
};

#endif // NEOSTRIP
