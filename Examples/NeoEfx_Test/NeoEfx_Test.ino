/* 
 * NeoEfx_Test- by Jerry Isdale
 *  NeoEffects is available at https://github.com/MauiJerry/NeoEffects_library
 *  this is a test tool steps thru each of the internally defined effects
 */

#include <Adafruit_NeoPixel.h>
#include "NeoEffects.h"

//////////////////////////////////////////
#define INTENSITY 255      //for NeoPixels
#define STRIP_SIZE 44
#define TIP_SIZE 4
#define WINDOW_SIZE (STRIP_SIZE - TIP_SIZE)
#define STRIP_1_PIN 13
#define DEFAULT_DELAYTIME 100

uint32_t
BLANK = Adafruit_NeoPixel::Color(0, 0, 0),
BLUE = Adafruit_NeoPixel::Color(0, 0, INTENSITY),
WHITE = Adafruit_NeoPixel::Color(INTENSITY, INTENSITY, INTENSITY),
RED = Adafruit_NeoPixel::Color(INTENSITY, 0, 0),
GREEN = Adafruit_NeoPixel::Color(0, INTENSITY, 0),
YELLOW = Adafruit_NeoPixel::Color(INTENSITY, INTENSITY, 0),
BROWN = Adafruit_NeoPixel::Color(165, 42, 0),
ORANGE = Adafruit_NeoPixel::Color(INTENSITY, 128, 0);

const int WIN_START = 1;
const int TIP_START = (WIN_START + WINDOW_SIZE);
const int numWindows = 2;

int defaultEfxCount = 10;
int poleNumber = 10; // id of the pole, used by tip flasher

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
NeoWindow tip = NeoWindow(&strip1, TIP_START, TIP_SIZE);

const uint32_t aNicePurple = strip1.Color(128, 0, 50);

int StartWinState = 1; // a hack to skip N steps when testing
int tipState = 0;
int windowState = StartWinState;

//////////////////////////////////////////
void setup() {

   // use serial line for debugging output
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.println("Starting NeoEffects Effects Test");

  // start the strip.  do this first for all strips
  strip1.begin();
  NeoWindow::updateTime();
  window.printId();

// NeoPixels can be very bright, and at full power can use lots of power
// longer 'strips' require extra power to run full bright. 
// brightness runs 0-255 and scales all colors to match that dark->bright
// strip1.setBrightness(100);

  blinkWholeStrip();

  // initially use built in efx, later update to new efx
  window.fillColor(GREEN );
  window.setHoldEfx(500);
    
  Serial.println("Update End of Setup");
//
//  window.updateWindow();

  strip1.show();
  Serial.println("Begin Loop");
}

void loop() {
  // grab the current time in class method
  NeoWindow::updateTime();
  
  int effectCount = defaultEfxCount; // changes per effect
  int delayTime = DEFAULT_DELAYTIME; // default is 1/4sec each step
  
  // window state machine
  if (window.effectDone()) {
    windowState++;
    Serial.print("window state:");Serial.print(windowState);
    switch (windowState) {
      //1 = SolidColorEfx
      // 2 = circle fwd
      // 3 = circle rev
      // 4 = wipe
      // 5 = randomWipe
      // 6 = reverseWipe
      // 7 = Blink
      // 8 = sparkle
      // 9 = MultiSparkle
      // 10 = Fade Once
      // 11 = Fade Cycle
      // 12 = Fade Jump
      // 13 = rainbow 255 len
      // 14 = rainbow even distrb
      // 15 = ?
      case 1:
        Serial.println(" Solid BLUE hold 2 sec");
        delayTime = 2000;
        window.setSolidColorEfx(BLUE, delayTime);
        break;
      case 2:
      {
        effectCount = 3;
        int tmpcolor = strip1.randomColor();
        Serial.print (" Circle fwd ");Serial.println(tmpcolor);
        window.setCircleEfx(tmpcolor, delayTime, effectCount);
        break;
      }
      case 3:
        Serial.println(" Circle rev");
         effectCount = 3;
       window.setCircleEfx(strip1.randomColor(), delayTime, effectCount, 1);
        break;
     case 4:
        Serial.println(" WipeEfx");
        window.setWipeEfx(aNicePurple, delayTime );
        break;
     case 5:
        Serial.println(" randomWipeEfx");
        window.setRandomWipeEfx(strip1.randomColor(), strip1.randomColor(), delayTime );
        break;
     case 6:
        Serial.println(" reverseWipe");
        window.setReverseWipeEfx(strip1.randomColor(), delayTime );
        break;
     case 7:
        Serial.println(" Blink");
        effectCount = 10;
        window.setBlinkEfx(aNicePurple, delayTime, effectCount);
        break;
     case 8:
        Serial.println(" Sparkle");
        effectCount = 20;
        window.setSparkleEfx(BLUE, delayTime, delayTime/2, effectCount);
        break;
     case 9:
        Serial.println(" MultiSparkle");
        effectCount = 20;
        window.setMultiSparkleEfx(aNicePurple, delayTime/2, delayTime, WINDOW_SIZE/5, effectCount);
        break;
    case 10:
        Serial.println(" Fade Once");
        window.setFadeEfx(0, strip1.randomColor(), 10, window.fadeTypeOnce); // fade between two colors
        break;
    case 11:
        Serial.println(" Fade Cycle");
        effectCount = 2; // default = once
        window.setFadeEfx(0, strip1.randomColor(), 10, window.fadeTypeCycle, effectCount); // fade between two colors
        break;
    case 12:
        Serial.println(" Fade JumpBack");
        effectCount = 2; // default = once
        window.setFadeEfx(0, strip1.randomColor(), 10, window.fadeTypeJumpBack, effectCount); // fade between two colors
        break;
    case 13:
        Serial.println(" Rainbow 255 len");
        window.setRainbowEfx(delayTime, 0);
        break;
    case 14:
        Serial.println(" Rainbow even distrib");
        Serial.println(" Rainbow 255 len");
        window.setRainbowEfx(delayTime, 1);
        break;
    default: // black for sanity
      // when windowState increments past highest case above, it falls into state = 0
        Serial.println(" DEFAULT = black hold 5sec");
        windowState = StartWinState;
        window.fillBlack( );
        window.setHoldEfx(5000);
    }
  }
  
  // now update each Window - does one 'frame' of effect on the window
  window.updateWindow();

  strip1.show();

//  if (window.effectDone()) 
//  {
//  Serial.println("Effect Done hold 20000");
//  delay(20000);
//  }
}


void blinkWholeStrip(void)
{
  Serial.println("Start blinkWholeStrip");
  strip1.clearStrip();
  strip1.show();
  delay(1000);    
  strip1.fillStrip(aNicePurple);
  strip1.show();
  delay(1000);
  strip1.clearStrip();
  strip1.show();
  delay(1000);
  Serial.println("End blinkWholeStrip");
}

