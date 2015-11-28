// NeoEffects library header... include the important components and dependencies
// NeoEffects provides multiple simultaneous lighting pattern effects on a NeoPixel (rgb led strip using ws2812)
// NeoPixel examples generally use delay() which precludes simultaneous effects

#ifndef NEOEFFECTS_LIBRARY
#define NEOEFFECTS_LIBRARY

// We are based on the Adafruit library: https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

// we add two classes:
// NeoStrip provides an abstraction for a daisy chain of NeoPixels, 
//    allowing multiple effects to share memory buffer and write only once per loop(), if needed
#include <NeoStrip.h>
// NeoWindow defines a window (range of pixels) within a strip on which an effect will be produced
//    windows can be distinct or overlap
//    a base set of effect functions is included in the library
//    additional effects can be implemented via subclassing
#include <NeoWindow.h>

#endif

