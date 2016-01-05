/* 
 * NeoEfx_Subclass - example showing how to create a subclass of NeoWindow to add new Effect
 *  
*/

#include <Adafruit_NeoPixel.h>
#include "NeoEffects.h"
#include "NeoRainbowEfx.h"

//////////////////////////////////////////
#define SMALL_NEORING_SIZE 16

#define STRIP_1_PIN 2

const int RING_1_START = 0;
const int RING_2_START = (RING_1_START + SMALL_NEORING_SIZE);
const int RING_3_START = (RING_2_START + SMALL_NEORING_SIZE);
const int RING_4_START = (RING_3_START + SMALL_NEORING_SIZE);
const int numRings = 4;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

NeoStrip strip1 = NeoStrip(SMALL_NEORING_SIZE * numRings, STRIP_1_PIN, NEO_GRB + NEO_KHZ800);

// first ring is same as BasicUse example
NeoWindow     ring1 = NeoWindow(&strip1, RING_1_START, SMALL_NEORING_SIZE);
// ring2 is subclass of Windows
NeoRainbowEfx ring2 = NeoRainbowEfx(&strip1, RING_2_START, SMALL_NEORING_SIZE);

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

  ring1.setWipeEfx(strip1.randomColor(),100 ); // wipe on a random color
  ring2.setRainbowEfx( 100, 0 );

}

int stateOne = 0;  // update to change ring1 effects
int stateTwo = 0;

void loop() {
  // grab the current time in class method
  NeoWindow::updateTime();

  // Simple wipe completed? chose another random color
  if (ring1.effectDone()) {
    stateOne++;
    switch (stateOne) {
      case 1:
        Serial.println("Ring1 Sparkle");
        ring1.setSparkleEfx(strip1.randomColor(),  50,  50, 100);
        break;
      case 2:
        Serial.println("Ring1 Fade");
        ring1.setFadeEfx(0, strip1.randomColor(), 100, ring1.fadeTypeCycle, 0); // fade between two colors
        break;
      case 3:
        Serial.println("Ring1 Circle");
        ring1.setCircleEfx(strip1.randomColor(), 200);
        break;
     default:
        Serial.println("Ring1 WipeEfx");
        stateOne = 0;
        ring1.setWipeEfx(strip1.randomColor(),100 );
    }
  }
  if (ring2.effectDone()) {
    if (stateTwo == 0) { 
      ring2.setRainbowEfx( 100, 1 );
      stateTwo = 1;
    } else {
      ring2.setRainbowEfx( 100, 0 );
      stateTwo = 0;
    }
  Serial.print("Rainbow State ");Serial.println(stateTwo);
  }
  
  if (ring2.getEffectCount() > 10) {
    ring2.setRainbowEfx( 100, 1 );
  }

    // now update each Window - does one 'frame' of effect on the window
  ring1.updateWindow();
  ring2.updateWindow();

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

