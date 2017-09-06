/*
   NeoEfx_BurningMan17 - testMultisparkle with random colors & background
*/

#include <Adafruit_NeoPixel.h>
#include "NeoEffects.h"
#define DEFAULT_DELAYTIME 100

//////////////////////////////////////////

// Comment out to turn off Serial and LED debugging indicators for better performance
#define DEBUG_MIN
#define DEBUG_STATE
//#define DEBUG_LED
//////////////////////////////////////////


#define STRIP_SIZE 50
#define WINDOW_SIZE STRIP_SIZE
#define INTENSITY 255      //for NeoPixels
#define STRIP_1_PIN 13
//////////////////////////////////////////

uint32_t
BLANK = Adafruit_NeoPixel::Color(0, 0, 0),
BLUE = Adafruit_NeoPixel::Color(0, 0, INTENSITY),
WHITE = Adafruit_NeoPixel::Color(INTENSITY, INTENSITY, INTENSITY),
RED = Adafruit_NeoPixel::Color(INTENSITY, 0, 0),
GREEN = Adafruit_NeoPixel::Color(0, INTENSITY, 0),
YELLOW = Adafruit_NeoPixel::Color(INTENSITY, INTENSITY, 0),
BROWN = Adafruit_NeoPixel::Color(165, 42, 0),
ORANGE = Adafruit_NeoPixel::Color(INTENSITY, 128, 0);

const int WIN_START = 0;
const int numWindows = 1;

int defaultEfxCount = 100;

//////////////////////////////////////////

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
NeoStrip strip1 = NeoStrip(STRIP_SIZE, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

// window & window use subclass ring3 is same as BasicUse example
NeoWindow window = NeoWindow(&strip1, WIN_START, WINDOW_SIZE);
//////////////////////////////////////////

const uint32_t aNicePurple = strip1.Color(128, 0, 50);

int StartWinState = 0;// negative one to hold initial Black state
int windowState = StartWinState;
int maxEfx = 255;

//////////////////////////////////////////
void setup() {
  // use serial line for debugging output
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window

  Serial.println("--Starting BM2017 TestMultiSparkle with BG color --");

  // start the strip.  do this first for all strips
  strip1.begin();

  // NeoPixels can be very bright, and at full power can use lots of power
  // longer 'strips' require extra power to run full bright.
  // brightness runs 0-255 and scales all colors to match that dark->bright
  // strip1.setBrightness(100);

  blinkWholeStrip();

  // initially use built in efx, later update to new efx
  // tip efx will quick flash its id number somehow
  //window.setRainEffect( numRainDrops, rainWaitTime, defaultEfxCount);
  window.fillBlack( );
  window.setHoldEfx(1000);
  // change to setTipEfx

  Serial.println("Begin Loop ... cur time: "); Serial.println(millis());
}

void loop() {

  // grab the current time in class method
  NeoWindow::updateTime();
  int delayTime = 250;
  int effectCount = 10; // 10 seconds
  // window state machine
  if (window.effectDone())
  {
    windowState++;
    if (windowState > maxEfx) {
      Serial.println("wrap around windowState");
      windowState = 0;
    }

    Serial.print("Effect Done, cur time: "); Serial.println(millis());
    Serial.print("   New State: "); Serial.println(windowState);
    int randNum = random(0,255);
    uint32_t color = strip1.colorWheel((byte)randNum);
    int tmp = randNum + 128;
     if (tmp > 255) tmp -= 255;
    uint32_t bgcolor = strip1.colorWheel((byte)tmp);
    Serial.printf("   randNum %d  opposite %d\n", randNum, tmp);
    Serial.printf("   Color =  0x%X BG: 0x%X \n", color, bgcolor);
    window.setBgColor(bgcolor);
    window.setMultiSparkleEfx(color, delayTime / 2, delayTime, 20, effectCount);
  }
  // now update each Window - does one 'frame' of effect on the window
  window.updateWindow();

  strip1.show();

}

//////////////////////////////////////////
void blinkWholeStrip(void)
{
  strip1.clearStrip();
  strip1.show();
  delay(1000);
  strip1.fillStrip(aNicePurple);
  strip1.show();
  delay(1000);
  strip1.clearStrip();
  strip1.show();
  delay(1000);
}

