# NeoEffects_library
NeoEffects provides classes to support multiple concurrent effects on NeoPixel strips/rings.

It is based on the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel).
[NeoPixels](http://www.adafruit.com/search?q=NeoPixel) are the Adafruit name for RGB LED devices that contain the WS2812 controller. The libraries should work well with other WS2812 devices (strips, rings, arrays, etc.)

Code includes DOxygen style documentation comments.

## Blink Without Delay()
The Adafruit NeoPixel Library example code, relies on the delay() function. This holds up the CPU and precludes multiple simultaneous effects on the same or different strips.  There are several techniques to avoid delay(). The principle technique is to use millis() to check elapsed time (see the examples from arduino.cc:  [BlinkWithoutDelay](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) and [AvoidDelay](http://playground.arduino.cc/Code/AvoidDelay) ).  

This library was created to simplify the use of the millis() technique on NeoPixel elements.

This library is a bit more complex (and functional) than the [Adafruit NeoPatterns](https://learn.adafruit.com/multi-tasking-the-arduino-part-3?view=all) or the DMA interrupt [Adafruit NeoPXL8](https://learn.adafruit.com/dma-driven-neopixels) or the [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)

https://learn.adafruit.com/dma-driven-neopixels)

# Library Design
The Library provides two classes: NeoStrip and NeoWindow

## NeoStrip Class
NeoStrip is a subclass of the Adafruit NeoPixel. The primary addition is a boolean to determine if the strip's memory buffer has been chnaged. If it has, then the buffer should be written using show(), otherwise the show() can be skipped.  Note that at present (v0.x) the NeoStrip does NOT override the pixel set functions and relies on higher level calls to setStripChanged(). This is to avoid overhead of lots of calls to set the boolean when changing a range of pixels.

NeoStrip also provides a boolean array of its pixels that can be used by effects to mark a pixel as Active or InActive.  This can be useful in some effects like Sparkle that have some subset of pixels active at a particular time.

The NeoStrip also provides some generic helper functions. clearStrip() and fillStrip() set all pixels in the strip to black or particular color. There are two randomColor() functions to generate random colors, and a set of functions (getRed(), getBlue(), getGreen()) to extract components from the 32bit composite color used by the WS2812 chip/NeoPixel library. It also provides the simple colorWheel() of 255 colors implemented in the strandtest example from the Adafruit_NeoPixel Library.

## NeoWindow Class
The NeoWindow class defines a range of pixels within a NeoStrip, and provides a set of Effects that operate on that range (window on a strip).  Note that windows are not exclusive and can overlap. They may or may not be active at any particular time. The last window updated will be the one shown.

There are a pair of class method, updateTime() and currentTime(), that are used to grab and check the current time during a single loop(), without repeated calls to millis().  There are some helper functions such as fillColor(), fillBlack(), and clearActive().

The programmer uses one of the setXXXEffect() methods to set the current effect for a window. This may be done in setup() or during loop in response to a change in inputs or other states.  An effect may run for a period and then complete, in which case it sets an internal Done flag, which can be checked with `effectDone()`.  Programmers can add new effects by subclassing NeoWindow.

## Effects Defined by NeoWindow

Currently NeoWindow defines the following effects:
- NoEffect : ignore the window; also used to reset internal timers/counters
- Hold : hold current pixel colors for time period
- Circle  : one dot of specified color moves across window with time period between changes
- Wipe  : wipe (fill) window one dot of color at a time, with delay between changes
- RandomWipe : wipe with random color in specified range
- Blink : blink all pixels in window on/off with specified color, rate and number blinks
- Sparkle : blink a random pixel in window with color (on/off time) then chose new pixel
- MultiSparkle : blink N random pixels in window on/off
- Fade : linear fade between two colors (once, cycling in/out, or jumping back to start)
- Morse: blinks out morse code string
- Dazzle: all pixels blink random bright colors

See the NeoWindow.h file for parameters used when starting these effects.

# Basic Use of NeoEffects
There are a few basic steps to use the NeoEffects (see *Examples/NeoEfx_BasicUse*).  First the entire strip needs to be declared/defined, exactly as you would do for a NeoPixel. Then you need to declare/define the NeoWindows giving their start pixel number and size...

```
// The strip is on arduino pin 1
#define STRIP_PIN 1

// we are using the smaller Adafruit NeoPixel with 12 leds
#define SMALL_NEORING_SIZE 12

// 4 rings on shoulder piece
const int RING_1_START = 0;
const int RING_2_START = (RING_1_START + SMALL_NEORING_SIZE);
const int RING_3_START = (RING_2_START + SMALL_NEORING_SIZE);
const int RING_4_START = (RING_3_START + SMALL_NEORING_SIZE);
const int numRings = 4;

// define the strip exactly like a NeoPixel object
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
NeoStrip strip1 = NeoStrip(SMALL_NEORING_SIZE * numRings, STRIP_PIN, NEO_GRB + NEO_KHZ800);

NeoWindow ring1 = NeoWindow(&strip1, RING_1_START, SMALL_NEORING_SIZE);
NeoWindow ring2 = NeoWindow(&strip1, RING_2_START, SMALL_NEORING_SIZE);
NeoWindow ring3 = NeoWindow(&strip1, RING_3_START, SMALL_NEORING_SIZE);
NeoWindow ring4 = NeoWindow(&strip1, RING_4_START, SMALL_NEORING_SIZE);
```
Then in the setup() method we need to initialize the strip. Optionally we may wish to start effects on the windows.
```
void setup()
{
  strip1.begin();
  ring1.setWipeEfx(strip1.randomColor(),100 ); // wipe on a random color
  ring2.setBlinkEfx(strip1.randomColor(), 250, 10); // blink a random color
  ring3.setCircleEfx(strip1.randomColor(), 200); // run one pixel across window
  ring4.setFadeEfx(0, strip1.randomColor(), 100, ring4.fadeTypeCycle, 0); // fade between two colors
}
```

The real magic happens in loop(). At the top of the loop, grab the current time, check for inputs/completed effects, update each window, and lastly show the strip.  The updateWindow() method internaly checks to see if the effect's time period is ready to update and if so updates pixels.

```
void loop()
{
  // grab the current time using the class method. thus it is only called once, regardless of # windows
  NeoWindow::updateTime();
  // Simple wipe completed? chose another random color
  if (ring1.effectDone())
      ring1.setWipeEfx(strip1.randomColor(),100 );
  // completed circle, pick new color
  if (ring3.effectDone())
      ring3.setCircleEfx(strip1.randomColor(), 200);

  // now update each Window - does one 'frame' of effect on the window
  ring1.updateWindow();
  ring2.updateWindow();
  ring3.updateWindow();
  ring4.updateWindow();

  // if the strip changed, send commands out to it.
  strip1.show();
}
```

# Adding new effects: Subclassing NeoWindow

Programmers can add their own effects by subclassing NeoWindow and providing a set and update function for each effect (as well as any supporting variables.  The *Examples/NeoEfx_Subclass* sketch shows how to implement the classic (overused) cycling rainbow effect.  A special programming 'trick' (function pointers) is used to hide details of updating. Function pointers can be tricky and perhaps hard to understand, but they are actually one of the more powerful parts of C and C++.

The public setXXX() method will initialize the effect variables and set the initial colors. It also sets the private member function pointer curUpdateFunc to the update function of the current effect.  Then when the updateWindow() method is called in loop() this updateXXX() function is called (but only if the current timer indicates such).

```
void NeoRainbowEfx:: setRainbowEfx(uint16_t waitTime)
{
  // members common to all effects
  setNoEfx();// resets common counters, timers etc
  effectDelay = waitTime;
  // effect specfic members
  curColor = 0;
  // and set the update function - magic of function poiners
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoRainbowEfx::rainbowEfxUpdate;
}
```

The effect specific update method does not need to worry about checking the time or marking the strip as changed. Those actions are handled by the updateWindow() method before it invokes the effect specific update.

# State Machines to change effects
Since we are no longer using a long list of effects and delay() in the loop(), we need an alternative way to control our system.

[State Machines](https://en.wikipedia.org/wiki/Finite-state_machine), aka finite state machines, is a programming technique that uses States and Transitions.  A simple state machine could test the value (state) of a LED_STATE variable and if it is on, turn the LED off (and change LED_STATE to off), and vice versa.  Sparkfun has a nice in-depth blog post about using [a state machine to control an LED](https://www.sparkfun.com/news/1801).

These are quite applicable to our new effects.  The libary's Neo_Effects_Example2 uses a simple state machine to control ring2

```
// outside and before loop() we define a variable to control the state:
int ring2State = false;

////// later in loop()

// two states: wipe on purple, wipe on black
  if (ring2.effectDone()){
    if (ring2State)
    {
      // wipe with black
      ring2.setWipeEfx(0,100 );
      ring2State = false;
    } else {
      // wipe with purple
      ring2.setWipeEfx(aNicePurple,100 );
      ring2State = true;
    }
  }
```

