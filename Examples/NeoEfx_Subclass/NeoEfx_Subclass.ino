/* 
 * NeoEfx_Subclass - example showing how to create a subclass of NeoWindow to add new Effect
 *  editor notes: while developing this, changes cant be saved back into the live
 *  Examples folder. So save to regular area and then copy back to Examples
*/

#include <Adafruit_NeoPixel.h>
#include "NeoEffects.h"
#include "NeoSubclassWindow.h"

//////////////////////////////////////////
#define NEORING_SIZE 16

#define STRIP_1_PIN 2

const int RING_1_START = 0;
const int RING_2_START = (RING_1_START + NEORING_SIZE);
const int RING_3_START = (RING_2_START + NEORING_SIZE);
const int numRings = 3;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

NeoStrip strip1 = NeoStrip(NEORING_SIZE * numRings, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

// ring1 & ring2 use subclass ring3 is same as BasicUse example
NeoSubclassWindow ring1 = NeoSubclassWindow(&strip1, RING_1_START, NEORING_SIZE);
NeoSubclassWindow ring2 = NeoSubclassWindow(&strip1, RING_2_START, NEORING_SIZE);
NeoWindow     ring3 = NeoWindow(&strip1, RING_3_START, NEORING_SIZE);

const uint32_t aNicePurple = strip1.Color(128, 0, 50);

//////////////////////////////////////////
void setup() {

   // use serial line for debugging output
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.println("Starting NeoEffects Subclass Rainbow Example");

  // start the strip.  do this first for all strips
  strip1.begin();

// NeoPixels can be very bright, and at full power can use lots of power
// longer 'strips' require extra power to run full bright. 
// brightness runs 0-255 and scales all colors to match that dark->bright
// strip1.setBrightness(100);

  blinkWholeStrip();

  ring1.setRainbowEfx( 100, 0 );
  ring2.setWipeEfx(strip1.randomColor(),100 ); // wipe on a random color
  ring3.setFadeEfx(0, aNicePurple, 100, ring1.fadeTypeCycle, 0); // fade between two colors
Serial.println("Begin Loop");
}

int stateOne = 0;  // update to change ring1 effects
int stateTwo = 0;

void loop() {
  // grab the current time in class method
  NeoWindow::updateTime();
  
  if (ring1.effectDone()) {
    if (stateOne == 0) { 
      ring1.setRainbowEfx( 100, 1 );
      stateOne = 1;
    } else {
      ring1.setRainbowEfx( 100, 0 );
      stateOne = 0;
    }
  }
  
  // ring2 state machine
  if (ring2.effectDone()) {
    stateTwo++;
    Serial.print("Ring2 state:");Serial.print(stateTwo);
    switch (stateTwo) {
      case 1:
        Serial.println(" Sparkle");
        ring2.setSparkleEfx(strip1.randomColor(),  50,  50, 100);
        break;
      case 2:
        Serial.println(" Fade");
        ring2.setFadeEfx(0, strip1.randomColor(), 10, ring1.fadeTypeCycle, 0); // fade between two colors
        break;
      case 3:
        Serial.println(" Circle");
        ring2.setCircleEfx(strip1.randomColor(), 200);
        break;
     case 4:
        Serial.println(" Blink");
        ring2.setBlinkEfx(aNicePurple, 250, 10);
        break;
     case 5:
        Serial.println(" MultiSparkle");
        ring2.setMultiSparkleEfx(aNicePurple, 250, 100,NEORING_SIZE/4,20);
        break;
     default:
        Serial.println(" WipeEfx");
        stateTwo = 0;
        ring2.setWipeEfx(strip1.randomColor(),100 );
    }
  }
    // now update each Window - does one 'frame' of effect on the window
  ring1.updateWindow();
  ring2.updateWindow();
  ring3.updateWindow();

  strip1.show();
}


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

