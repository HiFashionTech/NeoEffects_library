// MorseEfx - NeoEffect for morse code
// somewhat inspired by https://gist.github.com/madc/4474559
// change the include NewWin_Test.h to your app's NeoWin
// and copy the public/private parts to your class declaration
// be sure the naming of class is the same

// Note on Dot Time per Wikipedia
//  https://en.wikipedia.org/wiki/Morse_code
//  Speed in words per minute:  T = 1200/W
//  where T is dotTime and W is words per minute
//  a nice slow Boy Scout minimum is 5wpm
//  use this in your client that invokes setMorseEffect()


#include "NeoWindow.h"

//#define DEBUG_MORSE

//Build a struct with the morse code mapping
static const struct {const char letter, *code;} MorseMap[] =
{
  { 'A', ".-" },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.." },
  { 'E', "." },
  { 'F', "..-." },
  { 'G', "--." },
  { 'H', "...." },
  { 'I', ".." },
  { 'J', ".---" },
  { 'K', ".-.-" },
  { 'L', ".-.." },
  { 'M', "--" },
  { 'N', "-." },
  { 'O', "---" },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-." },
  { 'S', "..." },
  { 'T', "-" },
  { 'U', "..-" },
  { 'V', "...-" },
  { 'W', ".--" },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { ' ', "     " }, //Gap between word, seven units 
    
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" },
    
  { '.', "·–·–·–" },
  { ',', "--..--" },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ':', "---..." },
  { ';', "-.-.-." },
  { '(', "-.--." },
  { ')', "-.--.-" },
  { '"', ".-..-." },
  { '@', ".--.-." },
  { '&', ".-..." },
};
static const int morseMapSize = sizeof MorseMap / sizeof *MorseMap; // should be 48?

void NeoWindow::printMorseCurState()
{
  Serial.print("    morseCurState: ");Serial.print(morseCurState);
  switch (morseCurState){
    case sendDot: 
      Serial.println(" sendDot");
      break;
    case sendDash: 
      Serial.println(" sendDash");
      break;
    case sendOff: 
      Serial.println(" sendOff");
      break;
    case waitWord: 
      Serial.println(" waitWord");
      break;
    case tween: 
      Serial.println(" tween");
      break;
      default: 
      Serial.println(" UNKNOWN");
      break;
    
  }
}

void NeoWindow::setMorseEfx(uint32_t dotTime, uint32_t color, String msg, int repeatCount)
{
  setNoEfx();// reset counters
  morseDotTime = dotTime;
  effectDelay = dotTime;
  morseColor = color;
  // we dont really need to hold the TxtMessage, except for debugging
  String morseTxtMessage = String(msg);
  morseTxtMessage.toUpperCase(); // must be upper case 
  effectMaxCount = repeatCount;
  int txtLen = morseTxtMessage.length();
  morseCode.remove(0); // clear the CodeMessage
  // encode the message
  for (int i=0;i<txtLen;i++) {
    for(int j = 0; j < morseMapSize; ++j )
    {
      // slow lookup, but effective
      if( morseTxtMessage[i] == MorseMap[j].letter )
      {
        morseCode += MorseMap[j].code;
        break;
      }
    }
    morseCode += " "; //Add tailing space to seperate the chars
  }
  morseCodeLen = morseCode.length();
  morseCurCodeIdx = -1;
  morseCurState = tween;
  fillColor(0);
  effectDelay = morseDotTime;

#ifdef DEBUG_MORSE
  Serial.println("________");
  Serial.print("setMorseEfx dotTime:");Serial.print(morseDotTime);
      Serial.print(" color:");Serial.print(morseColor);
      Serial.print(" msg:");Serial.println(msg);
  Serial.print("      codeLen:"); Serial.println(morseCodeLen);
  Serial.print("      code:");Serial.println(morseCode);
  printMorseCurState();
#endif

  // and set the update function - magic of function poiners
  curUpdateFunc = (NeoWindowUpdateFunc) &NeoWindow::morseEffectUpdate;

}

void NeoWindow::morseEffectUpdate(void)
{ 
  // completed last delay
  // use morseCurState to control what happens
  // time unit is morseDotTime
  // time for dash is 3*unit
  // time tween parts of same letter is one unit
  // time tween characters is 3*unit
  // time tween words is 7 * unit
  // enum morseState {sendDot, sendDash, sendOff, waitWord} morseCurState;
#ifdef DEBUG_MORSE
  Serial.print("Morse update");printMorseCurState();
  Serial.print("      curCodeIdx: ");Serial.println(morseCurCodeIdx);
  Serial.print("      efxCount: ");Serial.println(effectCount);
#endif
 
  switch (morseCurState) {
    case sendDot:
    case sendDash:
    case waitWord:
      // turn off, wait dotTime
      fillColor(0);
      effectDelay = morseDotTime;
      morseCurState = sendOff;
      break;
    case tween:
    case sendOff:
      // advance to next Code
      morseCurCodeIdx++;
      if (morseCode[morseCurCodeIdx] == '.') {
        fillColor(morseColor);
        effectDelay = morseDotTime;
        morseCurState = sendDot;
        break;
      } else if (morseCode[morseCurCodeIdx] == '-') {
        fillColor(morseColor);
        effectDelay = morseDotTime*3;
        morseCurState = sendDash;
        break;
      } else if (morseCode[morseCurCodeIdx] == ' ') {
        fillColor(0);
        effectDelay = morseDotTime*3;
        morseCurState = waitWord;
        break;
      } else if (morseCode[morseCurCodeIdx] == 0) {
        morseCurState = tween;
        morseCurCodeIdx = -1;
        fillColor(0);
        effectDelay = morseDotTime*6; // delay tween restarts?
        morseCurState = sendOff;
        effectCount++;
        if (effectCount >= effectMaxCount) {
          efxDone = true;
          effectCount = 0;
        }
        break;
      }
      break; 
  }
  
#ifdef DEBUG_MORSE
  Serial.print("    __new ");printMorseCurState();
  Serial.print("      curCodeIdx: ");Serial.println(morseCurCodeIdx);
  Serial.print("      codeChar "); Serial.println(morseCode[morseCurCodeIdx]);
  Serial.print("      efxDone:" );Serial.println(efxDone);
  Serial.print("      efxCount: ");Serial.println(effectCount);
#endif
}


