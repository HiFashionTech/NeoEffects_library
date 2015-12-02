# NeoEffects_library
Tools to support multiple concurrent effects on NeoPixel strips/rings.
Based on the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel)
[NeoPixels](http://www.adafruit.com/search?q=NeoPixel) are the Adafruit name for RGB LED devices that contain the WS2812 controller. The libraries should work well with other WS2812 devices (strips, rings, arrays, etc.)

The example code, widely used as base by community, relies on the delay() function. This holds up the CPU and precludes multiple simultaneous effects on the same or different strips.  There are various examples of how to avoid delay() in general thru the use of millis() to check elapsed time (eg. [BlinkWithoutDelay](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) and [AvoidDelay](http://playground.arduino.cc/Code/AvoidDelay).  This library was created to simplify the use of this technique on NeoPixel elements.

The Library provides two classes: NeoStrip and NeoWindow
NeoStrip is a subclass of the Adafruit NeoPixel that primarily adds a boolean to determine if the strip's memory buffer has been chnaged. If it has, then the buffer should be written using show(), otherwise the show() can be skipped.  Note that at present (v0.x) the NeoStrip does NOT override the pixel set functions and relies on higher level calls to setStripChanged(). This is to avoid overhead of lots of calls to set the boolean when changing a range of pixels.
The NeoStrip also provides some helper functions.

Currently in early stages of moving from Glowaii prototyping to real library
Current code will not compile.

Intended use will be to create this as a library with several examples that can be pulled as a Zip and installed in arduino ide.
Examples are very incomplete at this point and need to be worked heavily before any use or real testing.
