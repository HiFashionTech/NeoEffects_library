// NeoWindow
/** @file NeoWindow.cpp
 @brief Implements the NeoWindow class of NeoEfx library - providing concurrent effects on NeoPixels
 @author jerry isdale
 */

#include "NeoStrip.h"
#include "NeoWindow.h"

//////
// to invoke the member function pointer...
//#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

// Class Variable definitions:

uint32_t NeoWindow::currTime = 0; // current Millis
int NeoWindow::s_nIDGenerator = 1;

/////////////////
// Constructor
NeoWindow::NeoWindow(NeoStrip *strip, int startPixel, int len)
{
  myId = s_nIDGenerator++;
  myStrip = strip;
  myStartPixel = startPixel;
  myPixelCount = len;
  myEndPixel = myStartPixel + myPixelCount-1;
    
    setNoEfx(); // this does rest of setup
}

void NeoWindow::printId(void)
{
  Serial.print("NeoWindow ");Serial.print(myId); Serial.print(" ");
}

void NeoWindow::printData(void)
{
  printId();
  Serial.print("   myStartPixel: ");Serial.println(myStartPixel);
  Serial.print("   myPixelCount: ");Serial.println(myPixelCount);
  Serial.print("   myEndPixel: ");Serial.println(myEndPixel);
  Serial.print("   efxDone: ");Serial.println(efxDone);
  Serial.print("   effectDelay: ");Serial.println(effectDelay);
//  Serial.print("   updateFunc: ");Serial.println((void *)curUpdateFunc);
  
}

void NeoWindow::updateWindow(void)
{
  if (!curUpdateFunc) {
//    printId(); Serial.println("    No updateFunc");
    return; // no effect defined. quick return
  }
  
   // determine if the current Effect time has passed

   if (NeoWindow::currTime - lastTime < effectDelay) {
//    printId(); Serial.println("    Not yet time");
    return;
   }
   lastTime = NeoWindow::currTime;
   
//   printId(); Serial.println(" invoke current effect function update");
   // invoke current effect function update
   (this->*curUpdateFunc)();

   myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::fillColor(uint32_t color)
{

  for (int i=myStartPixel; i <= myEndPixel; i++)
       myStrip->setPixelColor(i, color); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed

}

void NeoWindow::fillBlack()
{
    fillColor(0);
}

////////////////////////////////
// Effects here
void NeoWindow::setNoEfx()
{
  efxDone = false;
  effectDelay = 0;
  curUpdateFunc = NULL;
  effectCount = 0;
  lastTime = 0;
}

void NeoWindow::setHoldEfx(int delayTime)
{
//  printId(); Serial.println("    set to use hold effect");
    setNoEfx();
    effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::holdUpdateEfx;
}

void NeoWindow::holdUpdateEfx(void) 
{
  // once we are called the hold time has passed so mark us as done
  efxDone = true;
    effectCount++;
}

///////////////
void NeoWindow::setCircleEfx(uint32_t color, uint32_t delayTime)
{
//  printId(); Serial.println("    set to use circle effect");
    setNoEfx(); // reset values
  effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::circleUpdateEfx;
  
  // starting a Circle Effect using color and time
  circle_color = color;
  circle_cursor = myStartPixel;

  // setup the initial frame
  myStrip->setPixelColor(circle_cursor, circle_color);
  for (int i=circle_cursor+1; i< myEndPixel; i++)
    myStrip->setPixelColor(i, 0); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::circleUpdateEfx(void)
{
  // we assume the update function has determined if it is time to call me
  
//  printData();
//  Serial.println("Updating Circle Effect");
//  Serial.print("   circle_cursor: ");Serial.println(circle_cursor);
   
  // circle moves a single pixel of circle_color around the virtual circle of the window
  // clear the currentPixel
  myStrip->setPixelColor(circle_cursor, 0);
  circle_cursor++;
  if (circle_cursor > myEndPixel) { 
     circle_cursor = myStartPixel;
     efxDone = true; // if we dont check, it just continues
      effectCount++;
//     Serial.println(" wrapped circle");
  }
  myStrip->setPixelColor(circle_cursor, circle_color);
}

void NeoWindow::setWipeEfx(uint32_t color, uint32_t delayTime) // Wipe color once around window
{
//  printId(); Serial.println("    set to use wipe effect");
    setNoEfx(); // reset values

    effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::wipeUpdateEfx;
  
  // starting a Circle Effect using color and time
  wipe_color = color;
  wipe_cursor = myStartPixel;

  myStrip->setPixelColor(wipe_cursor, wipe_color);
  // dont change rest of colors
//  for (int i=circle_cursor+1; i< myEndPixel; i++)
//    myStrip->setPixelColor(i, 0); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::wipeUpdateEfx(void)
{
   // wipe fills the window one pixel each update, then sets Done
  
  myStrip->setPixelColor(wipe_cursor, wipe_color);
  wipe_cursor++;
  
  if (wipe_cursor > myEndPixel) { 
     efxDone = true;
     wipe_cursor = myStartPixel;
      
  }
}

//static uint32_t randomColor(uint32_t fromColor, uint32_t toColor)
//{
//  uint8_t fromR, toR;
//  uint8_t fromG, toG;
//  uint8_t fromB, toB;
//  fromR = NeoStrip::getRed(fromColor);
//  fromG = NeoStrip::getGreen(fromColor);
//  fromB = NeoStrip::getBlue(fromColor);
//  
//  toR = NeoStrip::getRed(toColor);
//  toG = NeoStrip::getGreen(toColor);
//  toB = NeoStrip::getBlue(toColor);
//  
//  return Adafruit_NeoPixel::Color(random(fromR,toR), random(fromG,toG),random(fromB,toB));
//}

void NeoWindow::setRandomWipeEfx(uint32_t color1, uint32_t color2, uint32_t delayTime) // Wipe color once around window
{
//  printId(); Serial.println("    set to use wipe effect");
    setNoEfx(); // reset values
    
  effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::randomWipeUpdateEfx;
  
  // starting a Circle Effect using color and time
  randomWipeColor1 = color1;
  randomWipeColor2 = color2;
  wipe_color = NeoStrip::randomColor(color1, color2);
  wipe_cursor = myStartPixel;

  myStrip->setPixelColor(wipe_cursor, wipe_color);
  // dont change rest of colors
//  for (int i=circle_cursor+1; i< myEndPixel; i++)
//    myStrip->setPixelColor(i, 0); //set rest of window to black
    
  myStrip->setStripChanged(); // mark the strip changed
}


void NeoWindow::randomWipeUpdateEfx(void)
{
   // wipe fills the window one pixel each update, then sets Done
  
  myStrip->setPixelColor(wipe_cursor, wipe_color);
  wipe_cursor++;
  
  if (wipe_cursor > myEndPixel) { 
     wipe_cursor = myStartPixel;
     wipe_color = NeoStrip::randomColor(randomWipeColor1, randomWipeColor2);
     efxDone = true;
  }
}

void NeoWindow::setBlinkEfx(uint32_t color, uint32_t delayTime, int count)
{
  // set the instance stuff
    setNoEfx(); // reset values
    
  effectDelay = delayTime;
  curUpdateFunc = &NeoWindow::blinkUpdateEfx;

  // effect specific stuff
  blink_color = color;
  blink_state = false;
  blink_maxCount = count;
    effectCount++;
  
  blinkUpdateEfx();

}

void NeoWindow::blinkUpdateEfx()
{
  if (blink_state)
  {
      fillColor(blink_color);
      blink_state = false;
  } else {
      fillBlack();
      blink_state = true;
      effectCount++;
  }
  
  if (blink_maxCount > 0 && effectCount > blink_maxCount)
    efxDone = true;
}

// couple flags for the state
static const int sparkleFLASH = 1;
static const int sparkleTWEEN = 0;

void NeoWindow::setSparkleEfx(uint32_t color, int flashTime, int tweenTime, int count)
{
    setNoEfx(); // reset values
    
  effectDelay = flashTime;
  curUpdateFunc = &NeoWindow::sparkleEfxUpdate;

  sparkleColor = color;
  sparkleFlashTime = flashTime;
  sparkleTweenTime = tweenTime;
  sparkleMaxCount = count;
  sparkleState = sparkleFLASH;
  sparkleCurPixel= random(myStartPixel, myEndPixel);
  fillBlack(); // clear it
  
  // now turn on just that pixel
  myStrip->setPixelColor(sparkleCurPixel,sparkleColor);
  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::sparkleEfxUpdate(void)
{
  if (sparkleState == sparkleFLASH) {
    // it is on, turn off and set to sparkleTWEEN
    myStrip->setPixelColor(sparkleCurPixel, 0);
    sparkleState = sparkleTWEEN;
    effectDelay = sparkleTweenTime;
  } else {
    // it is in TWEEN, so turn to FLASH: select new pixel and turn it on
    sparkleCurPixel= random(myStartPixel, myEndPixel);
    myStrip->setPixelColor(sparkleCurPixel, sparkleColor);
    sparkleState = sparkleFLASH;
    effectDelay = sparkleFlashTime;
  }

    effectCount++;
  if (sparkleMaxCount > 0 && effectCount > sparkleMaxCount)
  {
    efxDone = true;
  }
}
////////////////////////////////
void NeoWindow::clearActive(void)
{
  for (int i= myStartPixel; i <= myEndPixel; i++)
    myStrip->setPixelInactive(i);
}

/*! \brief setMultiSparkEfx: N randomly selected pixels in window flash on/off in selected color
 *  flashTime is time pixels are ON; Tween Time is time the are off
 *  when tweenTime is up, new pixels are chosen
 *  when there have been count flashes, the effect is set to Done.
 *
 */
void NeoWindow::setMultiSparkleEfx(uint32_t color, int flashTime, int tweenTime, int numActive, int count)
{
    setNoEfx(); // reset values
    
  effectDelay = flashTime;
  curUpdateFunc = &NeoWindow::multiSparkleEfxUpdate;

  multiSparkleColor = color;
  multiSparkleFlashTime = flashTime;
  multiSparkleTweenTime = tweenTime;
  multiSparkleMaxCount = count;
  multiSparkleNumActive = numActive;
  multiSparkleState = sparkleFLASH;
  
  if (numActive <= 0 || numActive > myPixelCount)
    numActive = myPixelCount;
  multiSparkleNumActive = numActive;

  
  clearActive();
  fillBlack(); // clear it

  // Select N pixels, make them Active and set them to the color
  multiSparkleEfxSelectPixels();

  myStrip->setStripChanged(); // mark the strip changed
}

void NeoWindow::multiSparkleEfxSelectPixels()
{
//  Serial.println("multiSparkleEfxSelectPixels");
  
    if (multiSparkleNumActive == 0 || multiSparkleNumActive == myPixelCount) {
        // do all pixels in window; skip random choice so it runs fast
        for (int idx=myStartPixel; idx<= myEndPixel; idx++) {
            myStrip->setPixelActive(idx);
            myStrip->setPixelColor(idx,multiSparkleColor);
        }
    } else {
        for (int i = 0; i < multiSparkleNumActive; i++){
          int idx = random(myStartPixel,myEndPixel);
          while (myStrip->isPixelActive(idx))
              // if it is already active, pick another; note this could take a while to fill large number
              idx = random(myStartPixel,myEndPixel);
    //      Serial.print("Set Pixel Active: "); Serial.print(idx);Serial.println();
          myStrip->setPixelActive(idx);
          myStrip->setPixelColor(idx,multiSparkleColor);
        }
  }
}

void NeoWindow::multiSparkleEfxUpdate(void)
{
//  printId(); Serial.print("multiSparkle Update state:"); Serial.print(multiSparkleState); Serial.println();
  
  if (multiSparkleState == sparkleFLASH) {
//    Serial.println("  clear and set to TWEEN");
    // it is on, turn off and set to multiSparkleTWEEN
    for (int idx = myStartPixel; idx <= myEndPixel;idx++) {
      if (myStrip->isPixelActive(idx))
        myStrip->setPixelColor(idx, 0);
    }
    clearActive();
    multiSparkleState = sparkleTWEEN;
    effectDelay = multiSparkleTweenTime;
  } else {
    // it is in TWEEN, so turn to FLASH: select new pixels
    multiSparkleEfxSelectPixels();
    
    multiSparkleState = sparkleFLASH;
    effectDelay = multiSparkleFlashTime;
  }

    effectCount++;
  if (multiSparkleMaxCount > 0 && effectCount > multiSparkleMaxCount)
  {
    efxDone = true;
  }

//  Serial.println("Bottom sparkle update"); 
//  printData();
//  delay(2000);
  
}

////////////////////
// Fade = linear fade between two colors, cycle makes if fade back
// linear fade in rgb space looks odd if not to black
//  better to fade in hsv or hcl
//  http://www.alanzucconi.com/2016/01/06/colour-interpolation/
//  but compute is expensive. might be better to precompute
//
static const int fadeFadeIn = 0;
static const int fadeFadeOut = 1;

void NeoWindow::setFadeEfx(uint32_t fromColor, uint32_t toColor, int fadeTime, int type, int count)
{
    setNoEfx(); // reset values
    
  effectDelay = fadeTime;
  curUpdateFunc = &NeoWindow::fadeEfxUpdate;

  fadeFromColor = fromColor;
  fadeToColor = toColor;
  fadeType = type;
  fadeMaxCount = count;
  
  // internally we use seperate RGB values
   fadeFromR = myStrip->getRed(fromColor);
   fadeFromG = myStrip->getGreen(fromColor);
   fadeFromB = myStrip->getBlue(fromColor);
  
   fadeToR = myStrip->getRed(toColor);
   fadeToG = myStrip->getGreen(toColor);
   fadeToB = myStrip->getBlue(toColor);

  fadeCurR = fadeFromR;
  fadeCurG = fadeFromG;
  fadeCurB = fadeFromB;

  fadePhase = fadeFadeIn;
  fillColor(fromColor);
  
  myStrip->setStripChanged(); // mark the strip changed
}


void NeoWindow::fadeEfxUpdate(void)
{
  // uses ternary operator to handle incr/decr direction
  if (fadePhase == fadeFadeIn){
    // fade in, fancy way to linearly ramp each of RGB, regardless of whether from>to or from<to
    fadeCurR = ((fadeToR > fadeCurR) ? (fadeCurR+1) : ((fadeToR != fadeCurR) ? fadeCurR-1 : fadeCurR));
    fadeCurG = ((fadeToG > fadeCurG) ? (fadeCurG+1) : ((fadeToG != fadeCurG) ? fadeCurG-1 : fadeCurG));
    fadeCurB = ((fadeToB > fadeCurB) ? (fadeCurB+1) : ((fadeToB != fadeCurB) ? fadeCurB-1 : fadeCurB));

    // faded all the way in? are we cycling?
    if (fadeCurR == fadeToR && fadeCurG == fadeToG && fadeCurB == fadeToB) {
      switch (fadeType) {
        case fadeTypeCycle:
          printId(); Serial.println("FadeEfx: cycled all the way in; fade out");
          // faded all in, cycle out
          fadePhase = fadeFadeOut;
          break;
        case fadeTypeJumpBack:
          printId(); Serial.println("FadeEfx: cycled all the way in; not cycle, jump back");
          fadeEfxEndCheck();
          fillColor(fadeFromColor);
          fadeCurR = fadeFromR;
          fadeCurG = fadeFromG;
          fadeCurB = fadeFromB;
        case fadeTypeOnce:
        default:
          //thats all
          break;
      }
    }
  } else { // fade out
    // fade in, fancy way to linearly ramp each of RGB, regardless of whether from>to or from<to
    fadeCurR = ((fadeFromR > fadeCurR) ? (fadeCurR+1) : ((fadeFromR != fadeCurR) ? fadeCurR-1 : fadeCurR));
    fadeCurG = ((fadeFromG > fadeCurG) ? (fadeCurG+1) : ((fadeFromG != fadeCurG) ? fadeCurG-1 : fadeCurG));
    fadeCurB = ((fadeFromB > fadeCurB) ? (fadeCurB+1) : ((fadeFromB != fadeCurB) ? fadeCurB-1 : fadeCurB));

    // faded all the way out?
    if (fadeCurR == fadeFromR && fadeCurG == fadeFromG && fadeCurB == fadeFromB) {
      printId(); Serial.println("FadeEfx: cycled all the way out; fade in");
      fadePhase = fadeFadeIn;
      fadeEfxEndCheck();
    }
  }
   fillColor(myStrip->Color(fadeCurR, fadeCurG, fadeCurB));
 
}

void NeoWindow::fadeEfxEndCheck()
{
    effectCount++;
  if (effectCount > fadeMaxCount)
  {
    efxDone = true;
  }
}



void NeoWindow:: setRainbowEfx(uint16_t waitTime, int type)
{
    // members common to all effects
    setNoEfx();// reset counters
    effectDelay = waitTime;
    // effect specfic members
    curColor = 0;
    rainbowType = type;
    
    // and set the update function - magic of function poiners
    curUpdateFunc = &NeoWindow::rainbowEfxUpdate;
}

void NeoWindow::rainbowEfxUpdate(void)
{
    // put a color in each pixel of window, depending on type
    for (int i=0; i < myPixelCount; i++)
    {
        // colorWheel has 255 colors, from Adafruit NeoPixel strandtest example
        if (rainbowType == 0) {
            myStrip->setPixelColor(i+myStartPixel, NeoStrip::colorWheel((i+curColor) & 255));
        } else {
            myStrip->setPixelColor(i+myStartPixel, NeoStrip::colorWheel(((i*256/myPixelCount)+curColor) & 255));
        }
    }
    
    curColor++;
    if (curColor > 255)
    {
        // reached end of colors, mark effect done
        effectCount++;
        efxDone = true;
        // and set color index back to start so we can keep cycling
        curColor = 0;
    }
}

