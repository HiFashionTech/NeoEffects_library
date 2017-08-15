// NeoWindow
/** @file NeoWindow.h
 * @brief NeoWindow class of NeoEfx library - providing concurrent effects on NeoPixels
 *
 * NeoWindow defines a Window (region) of a NeoPixel strip and concurrent effects to be performed.
 * Effects defined in different NeoWindows can be run concurrently, that is without use of delay()
 * A NeoWindow is defined on a NeoStrip (also of NeoEfx library).
 * Windows in a strip can be overlapping, although this can cause odd issues. Use SetNoEfx()
 *
 * Various effects are implemented by a couple functions: setXXXEfx(); updateXXXEfx()
 * the Set Efx function sets up the window to run the effect, variations will take various parameters
 * the update efx function is internal to the libary, called when an object's updateWindow() is called.
 *
 * @author jerry isdale
 */

#ifndef _NEOWINDOW_
#define _NEOWINDOW_

// all based on the NeoPixel library from Adafruit
#include <AdaFruit_NeoPixel.h>

// and our NeoStrip object that encapsulates the Pixel
#include "NeoStrip.h"

class NeoWindow 
{
protected:  
// class variables
  // for use herein, and by derived classes
    typedef void (NeoWindow::*NeoWindowUpdateFunc)(void); /*!< function that runs effect each cycle */
    static uint32_t currTime; /*!< current Millis */
    static int s_nIDGenerator; /*!<  used to generate uniqueID for each window */

public:
  // Instance Constructor:
  NeoWindow(NeoStrip *strip, int startPixel, int len); /*!< creates window on strip given start index & length */

  // Class functions
  static void updateTime() {NeoWindow::currTime = millis();}  /*!< class method to be called at top of loop() */
  static uint32_t currentTime() {return NeoWindow::currTime;} /*!< access time recorded at top of loop() */

  /////////////////////////
  // instance functions

  void updateWindow(void); /*!< invoke the current effect update function */
  boolean effectDone(void) {return efxDone;}        /*<! current effect has completed designated run */
  int getEffectCount(void) {return effectCount;}    /*!< returns count length of current effect */
  
  void printId(void);   /*<! uses Serial.println to emit ID of this window */
  void printData(void);   /*<! uses Serial.println to emit debug data on window/effect */
    
  void fillColor(uint32_t color);    /*<! fill window with given 32bit Adafruit_NeoPixel::Color */
  void fillBlack();     /*!< fill window with black == clear window */

  void clearActive(); /*!< clear Active flag for all pixels in my range */

// hidden Instance Functions
protected:
  /// pointer to the current effect's update function encapsulating the effect variables, etc
  NeoWindowUpdateFunc curUpdateFunc;
  
  /// Instance vars & methods
  NeoStrip *myStrip; /*!< the strip I belong to */
  uint16_t myId; /*!< for idenfying the window */
  uint16_t myStartPixel; /*!< absolute index of start pixel in this window */
  uint16_t myPixelCount;  /*!< number of pixels in this window */
  uint16_t myEndPixel;    /*!< absolute index of last pixel in this window */

  boolean efxDone;  /*!< set when current effect is complete, if ever */
  uint16_t effectCount; /*!< counter for number times efect has been updated */

  uint32_t lastTime; /*!< the last time current effect updated */
  uint32_t effectDelay; /*!< delay between updates of current effect */

  
////////////////////////////////////
// Different effects go here
// instance variables to support specific effects
// cleaner design might move these into some sort of Helper Class
// for each effect there is a public/private section
// public section defines the setXXXEfx function, and maybe exposes some stuff
// private section hides the effect function's instance variables
//// there should be some OO techinque for better implmenting this but sometime for later
   
// addtional code for other efx later or in subclass
//  void setPongEfx(uint32_t color, uint32_t delayTime);
//  void setRainbowEfx(uint32_t delayTime);

public:
  void setNoEfx();  /*!< ignore this window, resets all counters, timers, etc */
  void setHoldEfx(int delayTime); /*!< hold current colors for delayTime */
    
private:
  void holdUpdateEfx(void); /*!< update function for Hold Efx */

public:
    /**
     * @brief Circle effect: one pixel of given color repeatedly runs length of window
     * NeoEffect that moves a single colored pixel (rest black) thru window. on Rings this makes it circle
     * effect marked Done after One cycle
     * @param color an Adafruit_NeoPixel::Color to be used
     * @param delayTime time between pixel changes
     */
  void setCircleEfx(uint32_t color, uint32_t delayTime); /*!< cycle one pixel of color around window, moving one pixel each delayTime msec */
    
private:
  // circle Effect
  void circleUpdateEfx(void); /*!< private neoEfx update function for circle efx */
  uint32_t circle_color; /*!< private member holding color of circle effect */
  int circle_cursor; /*!< absolute index of currently active pixel within Strip */

public:
    /**
     * @brief Wipe Effect wipes color onto NeoWindow, one pixel at time gets color set
     * marked as Done after window is filled with color
     * @param color an Adafruit_NeoPixel::Color to be used
     * @param delayTime time between pixel changes
     */
  void setWipeEfx(uint32_t color, uint32_t delayTime); /*!< Wipe color once around window */
    /**
     * @brief Random Wipe Effect fills NeoWindow, one pixel at step with random color in range
     * marked as Done after window is filled with colors
     * @param color1 an Adafruit_NeoPixel::Color to be used as start of random range
     * @param color2 an Adafruit_NeoPixel::Color to be used as end of random range
     * @param delayTime time between pixel changes
     */
  void setRandomWipeEfx(uint32_t color1, uint32_t color2, uint32_t delayTime); /*!< Wipe color once around window */
    
private:
  // wipe once and then set Done
  void wipeUpdateEfx(void);
  void randomWipeUpdateEfx(void);
  uint32_t wipe_color; /*!< private member for color of wipe */
  int32_t wipe_cursor; /*!< private member for wipe postition */
  uint32_t randomWipeColor1; /*!< private member for start color of random wipe effect */
  uint32_t randomWipeColor2; /*!< private member for end color of random wipe effect */


public:
    /**
     * @brief Blink Effect toggles full window black/color with given delay and count of cycles
     * marked as Done after given number cycles; <= 0 means never end
     * @param color an Adafruit_NeoPixel::Color to be used
     * @param delayTime time between pixel changes
     * @param count number of times to blink window before marked Done; <= 0 means never stop
     */
  void setBlinkEfx(uint32_t color, uint32_t delayTime, int count = 0);

private:
  void blinkUpdateEfx(void);
  uint32_t blink_color;
  boolean blink_state;
  int blink_maxCount;

public:
    /**
     * @brief Sparkle Effect blinks a random pixel in given color, for duty cycle  and count of cycles
     * a new random pixel is set to color (rest black) each count.
     * marked as Done after given number cycles; <= 0 means never end
     * @param color an Adafruit_NeoPixel::Color to be used
     * @param flashTime time pixel is ON
     * @param tweenTime time pixel is OFF before next random one is turned on
     * @param count number of times to sparkle window before marked Done; <= 0 means never stop
     */
  void setSparkleEfx(uint32_t color, int flashTime, int tweenTime, int count = 0);
private:
  void sparkleEfxUpdate(void);
  uint32_t sparkleColor;
  int sparkleFlashTime;
  int sparkleTweenTime;
  int sparkleMaxCount;
  int sparkleCurPixel;
  int sparkleState;

public:
    /**
     * @brief MultiSparkle Effect blinks N random pixels in given color, for duty cycle  and count of cycles
     * a new random set of NumActive pixels are set to color and rest black each count.
     * marked as Done after given number cycles; <= 0 means never end
     * @param color an Adafruit_NeoPixel::Color to be used
     * @param flashTime time pixel is ON
     * @param tweenTime time pixel is OFF before next random set is turned on
     * @param numActive number of pixels turned on each step
     * @param count number of times to sparkle before marked Done; <= 0 means never stop
     */
  void setMultiSparkleEfx(uint32_t color, int flashTime, int tweenTime, int numActive = 0, int count = 0);
private:
  void multiSparkleEfxUpdate(void);
  void multiSparkleEfxSelectPixels();
  uint32_t multiSparkleColor;
  int multiSparkleFlashTime;
  int multiSparkleTweenTime;
  int multiSparkleMaxCount;
  int multiSparkleNumActive;
  int multiSparkleState;

// Fade = linear fade between two colors, cycle makes if fade back
public:
  static const int fadeTypeOnce = 0; /*!< fade effect: do it only once */
  static const int fadeTypeCycle = 1; /*!< fade effect: cycle between colors */
  static const int fadeTypeJumpBack = 2; /*< fade effect: jump back to starting color */
    /**
     * @brief Fade Effect fades window between two colors; can do once, cycle or jump
     * each update the color of window is stepped closer to end color
     * if type == 0 Done after 1 fade
     * if type == 1 fade back and forth between colors (each counts one)
     * if type == 2 jump back to first color when reach second
     *  fade steps are done by incre/decrementing RGB by 1 each step
     * marked as Done after given number cycles; <= 0 means never end
     * @param fromColor an Adafruit_NeoPixel::Color to be used as start
     * @param toColor an Adafruit_NeoPixel::Color to be used as end
     * @param fadeTime time between changes
     * @param type 0=Fade once 1= fade back& forth 2=jumpback to start color
     * @param count number of times to fade window before marked Done; <= 0 means never stop
     */
    
  void setFadeEfx(uint32_t fromColor, uint32_t toColor, int fadeTime, int type = fadeTypeCycle, int count = 0);

private:
  void fadeEfxUpdate(void);
  void fadeEfxEndCheck(void);
  uint32_t fadeFromColor;
  uint32_t fadeToColor;
  
  int fadeType;
  int fadeMaxCount;
  // internally we use seperate RGB values
  int fadeFromR;
  int fadeFromG;
  int fadeFromB;
  
  int fadeToR;
  int fadeToG;
  int fadeToB;

  int fadeCurR;
  int fadeCurG;
  int fadeCurB;

  boolean fadePhase;


public:
    // Rainbow moved to regular library function from subclass example
    /**
     * @brief Rainbow Effect ripples a rainbow of color thru window
     * each update the rainbow pattern moves one pixel
     * if type == 0 the rainbow is 255 pixels in length
     * if type == 1 the rainbow is spread across size of window
     * @param wait time between changes
     * @param type: 0=255 step rainbow; 1= evenly distributes color wheel across window size
     */
    void setRainbowEfx(uint16_t wait, int type); // type 0 or 1; 1 evenly distributes color

private:
    void rainbowEfxUpdate(void);
    int curColor;
    int rainbowType;
        
};

#endif
