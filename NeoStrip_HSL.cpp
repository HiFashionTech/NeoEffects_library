/**
 * NeoPixel HSL
 * 
 * Author: E. Scott Eastman
 * Date: Feb 2017
 * original GIT: https://github.com/Mazaryk/neopixel-hsl
 * 
 * Add HSL to RGB Color space conversion. Originially designed to 
 * work with the Adafruit NeoPixel Library
 * 
 * MIT License
 * Copyright (c) 2017 E. Scott Eastman (Mazaryk)
 * 
 * Author is not associated with Adafuit or NeoPixel
 * modified by Jerry Isdale for inclusion in NeoEffects as part of NeoStrip
 *
 * this is from Mazaryk's readme.md
# neopixel-hsl
NeoPixel HSL allows for HSL to RGB Colour Space conversion.

While using the awesome AdaFruit NeoPixels, I found it much more intuitive to use HSL (Hue, Saturation, Lightness) colour encoding instead of RGB (Red, Green, Blue). HSL makes it much easier to handle hue and brightness independently. This simplifies many common lighting effects, like fading a pixel from off to on, and keeping the colour consistent.

Also, this will make it much easier to say, hook up a potentiomiter and have it control only the hue. Using the pot, it's simple to wipe through the entire rainbow.

This implementation is designed for use with the NeoPixel Library, but should be easily modifiable to be used in other contexts. The hsl() function returns a 32 bit, packed RGB value, exaclty the same as Adafruit_NeoPixel::Color().

## Usage

### Inline

strip.setPixelColor(index, hsl(120, 100, 50)); // Gree, full saturation and brightness

### Assignment

uint32_t colour = hsl(0, 100, 25); // Red, full saturation, half brighness

### Good To Know
* Hue is specified in degrees, thus ranges from 0 - 359, Red = 0, Green = 120, Blue = 240
* Hue input is mod’d by 360, so 540 == 180
* Saturation is a percentage, ranges 0 - 100
* Lightness is a percentage, ranges 0 - 100
* Saturation and Lightness inputs are constrained, anything > 100 will be treated as 100.

## HSL Primer

Here are some notes/gotchas for those new to HSL, but familiar with RGB.

* Hue values should be stored in an int (or uint16_t), not a byte (or uint8_t)
* Saturation and lightness values can be stored in a byte (or uint8_t)
* The value returned by hsl() should be stored in a uint32_t
* A lightness value of 0 is off (same as RGB: 0,0,0) regardless of hue or saturation values
* A Lightness value of 50 is full colour lightness (Red in RGB 255,0,0 in HSL is 0,100,50)
* A Lightness value of 100 is always white, regardless of h or s values
* To fade a pixel from off to on, in only white, set saturation to 0, and increase lightness. Hue value is irrelevant.
*/
#include "NeoStrip.h"

//#define TEST_HSL_CODE
#ifdef TEST_HSL_CODE
// uncomment this block to test
#include <Adafruit_NeoPixel.h>

#define PIN 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); 
}

void loop() {
  demo_rotate();
//  demo_random();
}
*/
/**
 * Demo Rotate
 * 
 * Iterate through the hues, one pixel at time, 
 * and divide the strip into 3 equal parts.
 */
void demo_rotate() {

  static uint8_t n = strip.numPixels();
  
  static uint16_t hue = 0; // 0-359
  uint8_t saturation = 100; // 0-100
  uint8_t lightness = 50; // 0-100
  
  uint32_t color;
  uint8_t i; 
  
  for(i=0; i < n; i++) {
    color = hsl(hue++, saturation, lightness);
    strip.setPixelColor(i, color);
    strip.setPixelColor((i+n/3)%n, color);
    strip.setPixelColor((i+n/3*2)%n, color);
    strip.show();
    delay(40);
  }

  hue %= 360;
}
/**
 * Demo Random
 * 
 * Random Pixel, Random Color
 */
void demo_random() {

  uint8_t pixel = random(0, strip.numPixels());
  uint16_t hue = random(0, 360);
  uint8_t saturation = 100;
  uint8_t lightness = 50;
  
  strip.setPixelColor(pixel, hsl(hue, saturation, lightness));
  strip.show();
}
#endif // TEST_HSL_CODE

/////////////////////////////////////
/**
 * Map HSL color space to RGB
 * 
 * Totally borrowed from:
 * http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/ 
 * 
 * Probably not the most efficient solution, but 
 * it get's the job done.
 */
uint32_t NeoStrip::hsl(uint16_t ih, uint8_t is, uint8_t il) {

  float h, s, l, t1, t2, tr, tg, tb;
  uint8_t r, g, b;

  h = (ih % 360) / 360.0;
  s = constrain(is, 0, 100) / 100.0;
  l = constrain(il, 0, 100) / 100.0;

  if ( s == 0 ) { 
    r = g = b = 255 * l;
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  } 
  
  if ( l < 0.5 ) t1 = l * (1.0 + s);
  else t1 = l + s - l * s;
  
  t2 = 2 * l - t1;
  tr = h + 1/3.0;
  tg = h;
  tb = h - 1/3.0;

  r = hsl_convert(tr, t1, t2);
  g = hsl_convert(tg, t1, t2);
  b = hsl_convert(tb, t1, t2);

  // NeoPixel packed RGB color
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
/**
 * HSL Convert
 * Helper function
 */
uint8_t NeoStrip::hsl_convert(float c, float t1, float t2) {

  if ( c < 0 ) c+=1; 
  else if ( c > 1 ) c-=1;

  if ( 6 * c < 1 ) c = t2 + ( t1 - t2 ) * 6 * c;
  else if ( 2 * c < 1 ) c = t1;
  else if ( 3 * c < 2 ) c = t2 + ( t1 - t2 ) * ( 2/3.0 - c ) * 6;
  else c = t2;
  
  return (uint8_t)(c*255); 
}
