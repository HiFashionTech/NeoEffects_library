// Example to show basic use of the NeoEffects library: NeoStrip and NeoWindow classes
// we have four rings wired as a single strand and setup 4 simultaneous effects
//

#include <Adafruit_NeoPixel.h>
#include <NeoStrip.h>
#include <NeoWindow.h>

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
// strip1.setBrightness(180);
// strip1.setBrightness(200);

 // frist we Blink the whole strip to show app is running
  strip1.clearStrip();
  strip1.show();
  delay(1000);    
  strip1.fillStrip(Adafruit_NeoPixel::Color(255,255,255));
  strip1.show();
  delay(1000);
  strip1.clearStrip();
  strip1.show();
  delay(1000);
  
  // Set up initial effects
  ring1.setWipeEfx(strip1.randomColor(),100 );
  ring2.setBlinkEfx(strip1.randomColor(), 250, 10);
  ring3.setCircleEfx(strip1.randomColor(), 200);
  ring4.setFadeEfx(0, strip1.randomColor(), 100, NeoWindow::fadeTypeJumpBack, 0);

Serial.println("Setup Done");
}

int ring2State = false;

int ring3State = 0;

void loop() {
    
  // grab the current time in class method
  NeoWindow::updateTime();

  // check all inputs and update Effects per Window, etc
  if (ring1.effectDone())
      ring1.setWipeEfx(strip1.randomColor(),100 );
      
  if (ring2.effectDone()){
    if (ring2State)
    {
      ring2.setWipeEfx(0,100 );
      ring2State = false;
    } else {
      ring2.setWipeEfx(ourPurple,100 );
      ring2State = true;
    }
  }
  if (ring3.effectDone()) {
    switch (ring3State) {
      case 0:
        ring3.fillColor(ourPurple);
        ring3.setHoldEfx(5000);
        ring3State = 1;
        break;
      case 1:
        ring3.fillBlack();
        ring3.setHoldEfx(5000);
        ring3State = 2;
        break;
      case 2:
      default:
         ring3.setBlinkEfx(ourPurple, 250, 10);
         ring3State = 0;
    }
  }
  if (ring4.effectDone())
      ring4.setCircleEfx(strip1.randomColor(), 200);

  // now update each Window
  for (int i=0; i< numRings;i++)
  {
      rings[i]->updateWindow();
  }
  
//  ring1.updateWindow();
//  ring2.updateWindow();
//  ring3.updateWindow();
//  ring4.updateWindow();
//  ring5.updateWindow();
//  ring6.updateWindow();
//  ring7.updateWindow();
//  ring8.updateWindow();

  strip1.show();
//Serial.print("BottomLoop "); Serial.println(millis());
//  delay(1000); 
}
