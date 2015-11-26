// somewhat more elaborate example of the NeoStrip and NeoWindow classes
//

#include <Adafruit_NeoPixel.h>
#include "NeoStrip.h"
#include "NeoWindow.h"

#define SMALL_NEORING_SIZE 12

// Pins for strips connected via OctoWS2811
// these are assigned to odd ordering of pins for reasons internal to Octo library
// although we are not using the library, we may use the Octo interface board 
// which provides access to these pins via 2 RJ45 connectors.
//#define STRIP_1_PIN 2
// strip moved to pin 1 for shoulder piece
#define STRIP_1_PIN 1

// 6 rings on shoulder piece
const int RING_1_START = 0;
const int RING_2_START = (RING_1_START + SMALL_NEORING_SIZE);
const int RING_3_START = (RING_2_START + SMALL_NEORING_SIZE);
const int RING_4_START = (RING_3_START + SMALL_NEORING_SIZE);
const int RING_5_START = (RING_4_START + SMALL_NEORING_SIZE);
const int RING_6_START = (RING_5_START + SMALL_NEORING_SIZE);
const int numRings = 6;

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
NeoWindow ring5 = NeoWindow(&strip1, RING_5_START, SMALL_NEORING_SIZE);
NeoWindow ring6 = NeoWindow(&strip1, RING_6_START, SMALL_NEORING_SIZE);

NeoWindow *rings[] = {&ring1, &ring2, &ring3, &ring4, &ring5, &ring6};

int anoukRed = 128;
int anoukGreen = 0;
int anoukBlue = 50;

uint32_t anoukPurple = strip1.Color(128, 0, 50);

uint32_t randomAnouk() 
{
  return Adafruit_NeoPixel::Color(random(0,255),0,random(0,255));
}

void setup() {
  Serial.begin(115200);
  delay(500); // delay a bit when we start so we can open arduino serial monitor window
  
  Serial.println("Starting NeoEffects Test");
  
  strip1.begin();

 // if dont setBrightness down, we get odd effects.  probably because it draws too much power
// strip1.setBrightness(100);
// strip1.setBrightness(180);
// strip1.setBrightness(200);

 strip1.clearStrip();
 strip1.show();
  delay(1000);    
//  strip1.fillStrip(Adafruit_NeoPixel::Color(255,255,255));
  strip1.fillStrip(anoukPurple);  //Adafruit_NeoPixel::Color(128,128,128));
  strip1.show();
  delay(1000);
  strip1.clearStrip();
  strip1.show();
  delay(1000);
  
  // put your setup code here, to run once:
//  ring1.setWipeEfx(strip1.randomColor(),100 );
  ring1.setWipeEfx(randomAnouk(),100 );
  ring2.setWipeEfx(anoukPurple,100 );
  ring3.setBlinkEfx(anoukPurple, 250, 10);
  
  ring4.setCircleEfx(randomAnouk(), 200);
  ring5.setFadeEfx(0, anoukPurple, 100, ring5.fadeTypeJumpBack, 0);
//  ring5.setCircleEfx(Adafruit_NeoPixel::Color(128, 0, 128), 10);
  ring6.setSparkleEfx(anoukPurple, 10, 200);
//  
//  ring7.setFadeEfx(anoukPurple, strip1.Color(128,128,128), 10, ring7.fadeTypeCycle, 0);
////  ring7.setFadeEfx(anoukPurple, strip1.White, 10, ring7.fadeTypeCycle, 0);
////  ring7.setCircleEfx(Adafruit_NeoPixel::Color(0, 128, 128), 500);
//  ring8.setFadeEfx(0, anoukPurple, 1000, ring8.fadeTypeOnce, 0);
////  ring8.setCircleEfx(anoukPurple, 50);

Serial.println("Setup Done");
}

int ring2State = false;

int ring3State = 0;

void loop() {
    
  // grab the current time in class method
  NeoWindow::updateTime();

  // check all inputs and update Effects per Window, etc
  if (ring1.effectDone())
      ring1.setWipeEfx(randomAnouk(),100 );
      
  if (ring2.effectDone()){
    if (ring2State)
    {
      ring2.setWipeEfx(0,100 );
      ring2State = false;
    } else {
      ring2.setWipeEfx(anoukPurple,100 );
      ring2State = true;
    }
  }
  if (ring3.effectDone()) {
    switch (ring3State) {
      case 0:
        ring3.fillColor(anoukPurple);
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
         ring3.setBlinkEfx(anoukPurple, 250, 10);
         ring3State = 0;
    }
  }
  if (ring4.effectDone())
      ring4.setCircleEfx(randomAnouk(), 200);

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
