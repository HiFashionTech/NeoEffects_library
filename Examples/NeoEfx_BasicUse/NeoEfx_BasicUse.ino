// Example to show basic use of the NeoEffects library: NeoStrip and NeoWindow classes
// we have four rings wired as a single strand and setup 4 simultaneous effects
//

#include <Adafruit_NeoPixel.h>
#include <NeoEffects.h>

// we are using the smaller Adafruit NeoPixel with 12 leds
// http://www.adafruit.com/products/1643
#define SMALL_NEORING_SIZE 12

// Pins for strips connected via OctoWS2811
// these are assigned to odd ordering of pins for reasons internal to Octo library
// although we are not using the library, we may use the Octo interface board 
// which provides access to these pins via 2 RJ45 connectors.
//#define STRIP_1_PIN 2
// strip moved to pin 1 for shoulder piece
#define STRIP_1_PIN 1

// 4 rings on shoulder piece
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

NeoWindow ring1 = NeoWindow(&strip1, RING_1_START, SMALL_NEORING_SIZE);
NeoWindow ring2 = NeoWindow(&strip1, RING_2_START, SMALL_NEORING_SIZE);
NeoWindow ring3 = NeoWindow(&strip1, RING_3_START, SMALL_NEORING_SIZE);
NeoWindow ring4 = NeoWindow(&strip1, RING_4_START, SMALL_NEORING_SIZE);

void setup() {
  // use serial line for debugging output
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.println("Starting NeoEffects Test");

  // start the strip.  do this first for all strips
  strip1.begin();

// NeoPixels can be very bright, and at full power can use lots of power
// longer 'strips' require extra power to run full bright. 
// brightness runs 0-255 and scales all colors to match that dark->bright
// strip1.setBrightness(100);

 // first we Blink the whole strip to show app is running
  strip1.clearStrip();
  strip1.show();
  delay(1000);    
  strip1.fillStrip(Adafruit_NeoPixel::Color(255,255,255));
  strip1.show();
  delay(1000);
  strip1.clearStrip();
  strip1.show();
  delay(1000);
  
  // Now set up effects to run on each of 4 rings
  ring1.setWipeEfx(strip1.randomColor(),100 ); // wipe on a random color
  ring2.setBlinkEfx(strip1.randomColor(), 250, 10); // blink a random color
  ring3.setCircleEfx(strip1.randomColor(), 200); // run one pixel across window
  ring4.setFadeEfx(0, strip1.randomColor(), 100, ring4.fadeTypeCycle, 0); // fade between two colors

Serial.println("Setup Done");
}


void loop() {
  // Basic NeoEffects loop:
  // update time
  // check inputs & effectsDone - change behavior as needed
  // update each active Window
  // show the strips
  
  // grab the current time using the class method. thus it is only called once, regardless of # windows
  NeoWindow::updateTime();

  // here we might check inputs and EffectDone() to change effects

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
  
//Serial.print("BottomLoop "); Serial.println(millis());
//  delay(1000); 
}
