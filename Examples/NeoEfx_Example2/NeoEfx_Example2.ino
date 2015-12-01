// somewhat more elaborate example of the NeoStrip and NeoWindow classes
// using an array of Windows to simplify updates, 
// some more complex actions on Effects Done

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

// we create an array of ings (windows) to make updating simpler
NeoWindow *rings[] = {&ring1, &ring2, &ring3, &ring4 };

const uint32_t aNicePurple = strip1.Color(128, 0, 50);

void setup() {
  // use serial line for debugging output
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.println("Starting NeoEffects Example2");

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
  
  // put your setup code here, to run once:
  ring1.setWipeEfx(strip1.randomColor(),100 ); // wipe on a random color

  ring2.setWipeEfx(aNicePurple,100 );
  ring3.setBlinkEfx(aNicePurple, 250, 10);
  
  ring4.setCircleEfx(strip1.randomColor(), 200);

Serial.println("Setup Done");
}

// Ring 2 will have two states and implement a wipe on/wipe off effect
int ring2State = false;
// Ring 3 has three states 
int ring3State = 0;

void loop() {
    
  // grab the current time in class method
  NeoWindow::updateTime();

  // check all inputs and update Effects per Window, etc
  // ring1: wipe with random color
  if (ring1.effectDone())
      ring1.setWipeEfx(strip1.randomColor(),100 );

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

  // 3rd ring has 3 states: effectively On/Off and blinking
  if (ring3.effectDone()) {
    switch (ring3State) {
      case 0:
        ring3.fillColor(aNicePurple);
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
         ring3.setBlinkEfx(aNicePurple, 250, 10);
         ring3State = 0;
    }
  }
  // 4th ring, switch to new color
  if (ring4.effectDone())
      ring4.setCircleEfx(strip1.randomColor(), 200);

  // now update each Window, using the array
  for (int i=0; i< numRings;i++)
  {
      rings[i]->updateWindow();
  }
 
  // if the strip changed, send commands out to it.
  strip1.show();
  
//Serial.print("BottomLoop "); Serial.println(millis());
//  delay(1000); 
}
