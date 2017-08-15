/** @file NeoWindow.h
 * @brief NeoEffects implements concurrent effects on NeoPixel devices
 *  NeoEffects provides multiple simultaneous lighting pattern effects
 *    on a NeoPixel (rgb led strip using ws2812)
 *  NeoPixel examples generally use delay() which precludes simultaneous effects
 *  this library two classes:
 *  NeoStrip provides an abstraction for a daisy chain of NeoPixels,
 *    allowing multiple effects to share memory buffer and 
 *    write only once per loop(), if needed
 *  NeoWindow defines a window (range of pixels) within a strip 
 *    on which an effect will be produced
 *    windows can be distinct or overlap
 *    a base set of effect functions is included in the library
 *    additional effects can be implemented via subclassing NeoWindow
 * CAVEAT: latest NeoPixel can support RGBW strips. This library does NOT.
 */

#ifndef NEOEFFECTS_LIBRARY
#define NEOEFFECTS_LIBRARY

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

// We are based on the Adafruit library:
//  https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

// we add two classes in these headers
#include <NeoStrip.h>
#include <NeoWindow.h>

#endif

