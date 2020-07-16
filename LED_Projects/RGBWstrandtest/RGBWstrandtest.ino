#include <Adafruit_NeoPixel.h>
//#include <IRremote.h>
//#include <IRremoteInt.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>

/*
    Board: Arduino Nano
    Processor: ATmega328P (Old Bootloader)
    QinHeng Electronics HL-340 USB-Serial adapter
    lcd_I2C is on address 0x3f
*/

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6         // Data out pin for LED string
#define txPin 12      // comm output D12
#define rxPin 11      // comm input  D11

#define NUM_LEDS 30
#define BRIGHTNESS 70

#define varResistor1 0     // Analog input A0
#define varResistor2 1     // Analog input A1
#define hexSwitch1 9       // hex switch definitions
#define hexSwitch2 10      // hex switch definitions
#define hexSwitch4 11      // hex switch definitions
#define hexSwitch8 12      // hex switch definitions

int receiver = 8; // pin 1 of IR receiver to Arduino digital pin 11
int delayVal = 500;
int ar2 = 0;

int hexS1 = 0;
int hexS2 = 0;
int hexS4 = 0;
int hexS8 = 0;
int hexVal = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};
//-------------------------------------------------------

//Intialize the whole thing
void setup() {
 // lcd.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin (9600); //The following is for the comm port
  printDebug("setup");
  //These are pin definitions
  pinMode(varResistor1, INPUT);
  pinMode(varResistor2, INPUT);
  pinMode(hexSwitch1, INPUT_PULLUP);
  pinMode(hexSwitch2, INPUT_PULLUP);
  pinMode(hexSwitch4, INPUT_PULLUP);
  pinMode(hexSwitch8, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  Timer1.initialize(2e6);            // initialize timer1, and set a 2 second period
  //Timer1.pwm(13, 512);             // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
//-------------------------------------------------------
//Build a debug status string and print it if it has changed
void printDebug(String trace) {
  static String Saveit;

  if (Saveit != trace) {
    Serial.println(trace );
    Saveit = trace;
  }
}
//-------------------------------------------------------
//This function reads the variable resistors and hex switch
void callback()
{
  delayVal = analogRead(varResistor1);
  ar2 = analogRead(varResistor2);

  hexVal = 0;
  hexS1 = digitalRead(hexSwitch1);
  hexS2 = digitalRead(hexSwitch2);
  hexS4 = digitalRead(hexSwitch4);
  hexS8 = digitalRead(hexSwitch8);

  if (hexS1 == 0) {
    hexVal = 1;
  };
  if (hexS2 == 0) {
    hexVal = hexVal + 2;
  };
  if (hexS4 == 0) {
    hexVal = hexVal + 4;
  };
  if (hexS8 == 0) {
    hexVal = hexVal + 8;
  };

  String ts = (String(delayVal) + "  " + String(ar2) + " [" 
  + String(hexVal,BIN) + "  " + String(hexVal,HEX) + "  " + String(hexVal,DEC) + "]");

  printDebug(ts);
}
//-------------------------------------------------------
//This function reads a string from the user and also prints a trace string
void readInput(String trace = "?") {
  printDebug("readInput ----" + trace + "----");
  //This handles input from user mostly debug stuff
  String tStr =  Serial.readString();
  //BiteSerial.println(tStr );
  if (tStr.length() > 1) {
    Serial.print(tStr  + ' '); //don't add an extra newline
    tStr = "";
  }
}
//-------------------------------------------------------
//The main loop of the program
void loop() {
  //printDebug("----loop----");
  // Some example procedures showing how to display to the pixels:

  if  ((hexVal == 15) || (hexVal == 1)) {
    colorWipe(strip.Color(255, 0, 0)); // Red
  }
  if ((hexVal == 15) || (hexVal == 2)) {
    colorWipe(strip.Color(0, 255, 0)); // Green
  }
  if ((hexVal == 15) || (hexVal == 3)) {
    colorWipe(strip.Color(0, 0, 255)); // Blue
  }
  if ((hexVal == 15) || (hexVal == 4))  {
    colorWipe(strip.Color(0, 0, 0, 255)); // White
  }
  if ((hexVal == 15) || (hexVal == 5))  {
    whiteOverRainbow(20, 75, 5);
  }
  if ((hexVal == 15) || (hexVal == 6))  {
    pulseWhite(5);
  }
  if ((hexVal == 15) || (hexVal == 7))  {
    fullWhite();
  }
  if ((hexVal == 15) || (hexVal == 8))  {
    rainbowFade2White(3, 3, 1);
  }
  if ((hexVal == 15) || (hexVal == 9))  {
    theaterChaseRainbow(50);
  }
  if ((hexVal == 15) || (hexVal == 10))  {
    theaterChase(strip.Color(127, 127, 127), 50); // White
  }
  if ((hexVal == 15) || (hexVal == 11))  {
    theaterChase(strip.Color(127,   0,   0), 50); // Red
  }
  if ((hexVal == 15) || (hexVal == 12))  {
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  }
  if ((hexVal == 15) || (hexVal == 13))  {
    Colors8();
  }
  if ((hexVal == 15) || (hexVal == 14))  {
    Colors64K() ;
  }
}

//-------------------------------------------------------
// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  printDebug("colorWipe " + String(c));
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(delayVal);
  }
}
//-------------------------------------------------------
void Colors8() {
  printDebug("Colors8");
  for (int led = 0; led < NUM_LEDS  ; led++) { // Counts through the neopixels
    switch (random (0, 8) ) {
      case 0:
        strip.setPixelColor(led, strip.Color(255, 0, 0)); //red
        break;
      case 1:
        strip.setPixelColor(led, strip.Color(0, 255, 0)); //green
        break;
      case 2:
        strip.setPixelColor(led, strip.Color(0, 0 , 255)); //blue
        break;
      case 3:
        strip.setPixelColor(led, strip.Color(255, 255, 0)); //red green
        break;
      case 4:
        strip.setPixelColor(led, strip.Color(255, 0, 255)); //red blue
        break;
      case 5:
        strip.setPixelColor(led, strip.Color(0, 255, 255)); //green blue
        break;
      case 6:
        strip.setPixelColor(led, strip.Color(255, 255, 255)); //white
        break;
      case 7:
        strip.setPixelColor(led, strip.Color(0, 0, 0)); //black
        break;
    }
  }
  strip.show(); // This sends the updated pixel color to the hardware.
  delay(delayVal);
}
//-------------------------------------------------------
void Colors64K() {
  printDebug("Colors64k");
  for (int led = 0; led < NUM_LEDS  ; led++) { // Counts through the neopixels
    int a = random (0, 256);
    int b = random (0, 256);
    int c = random (0, 256);
    strip.setPixelColor(led, strip.Color(a, b, c));
  }
  strip.show(); // This sends the updated pixel color to the hardware.
  delay(delayVal);
}
//-------------------------------------------------------
void pulseWhite(uint8_t delayVal) {
  printDebug("pulseWhite");
  for (int j = 0; j < 256 ; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
    }
    delay(delayVal);
    strip.show();
  }

  for (int j = 255; j >= 0 ; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
    }
    delay(delayVal);
    strip.show();
  }
}

//-------------------------------------------------------
void rainbowFade2White(uint8_t delayVal, int rainbowLoops, int whiteLoops) {
  printDebug("rainbowFade2White");
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for (int k = 0 ; k < rainbowLoops ; k ++) {

    for (int j = 0; j < 256; j++) { // 5 cycles of all colors on wheel

      for (int i = 0; i < strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal / fadeMax);
        greenVal = green(wheelVal) * float(fadeVal / fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal / fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if (k == 0 && fadeVal < fadeMax - 1) {
        fadeVal++;
      }

      //Last loop, fade out!
      else if (k == rainbowLoops - 1 && j > 255 - fadeMax ) {
        fadeVal--;
      }

      strip.show();
      delay(delayVal);
    }

  }

  delay(delayVal);

  for (int k = 0 ; k < whiteLoops ; k ++) {

    for (int j = 0; j < 256 ; j++) {

      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
      }
      strip.show();
    }

    delay(delayVal);
    for (int j = 255; j >= 0 ; j--) {

      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, neopix_gamma[j] ) );
      }
      strip.show();
    }
  }

  delay(delayVal);
}

//-------------------------------------------------------
void whiteOverRainbow(uint8_t delayVal, uint8_t whiteSpeed, uint8_t whiteLength ) {
  printDebug("whiteOverRainbow");
  if (whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;

  while (true) {
    for (int j = 0; j < 256; j++) {
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ) {
          strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
        }
        else {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }

      }

      if (millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if (head == strip.numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if (loopNum == loops) return;

      head %= strip.numPixels();
      tail %= strip.numPixels();
      strip.show();
      delay(delayVal);
    }
  }
}

//-------------------------------------------------------
void fullWhite() {
  printDebug("fullWhite");
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0, 255 ) );
  }
  strip.show();
}
//-------------------------------------------------------

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t delayVal) {
  printDebug("rainbowCycle");
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(delayVal);
  }
}

//-------------------------------------------------------
void rainbow(uint8_t delayVal) {
  printDebug("rainbow");
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(delayVal);
  }
}

//-------------------------------------------------------
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t delayVal) {
  printDebug("theaterChase");
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(delayVal);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}
//-------------------------------------------------------
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t delayVal) {
  printDebug("theaterChaseRainbow");
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();
      digitalWrite(LED_BUILTIN, HIGH);
      delay(delayVal);
      digitalWrite(LED_BUILTIN, LOW);
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

/*

  digitalWrite(LED_BUILTIN, HIGH);
  delay(delayVal);
  digitalWrite(LED_BUILTIN, LOW);

*/
//-------------------------------------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

//-------------------------------------------------------
uint8_t red(uint32_t c) {
  return (c >> 16);
}

//-------------------------------------------------------
uint8_t green(uint32_t c) {
  return (c >> 8);
}

//-------------------------------------------------------
uint8_t blue(uint32_t c) {
  return (c);
}

//-------------------------------------------------------
/*
    for (int led = 0; led < NUMPIXELS ; led++) { // Counts through the neopixels
      //This handles input from user
      String tStr =  Serial.readString();
      if (tStr.length() > 1) {
        //Serial.print(tStr  + ' '); //don't add an extra newline
        if (tStr.indexOf("red") == 0) {
          pixels.setPixelColor(led, pixels.Color(255, 0, 0)); //red
        }
        else if (tStr.indexOf("green") == 0) {
          pixels.setPixelColor(led, pixels.Color(0, 255, 0)); //green
        }
        else if (tStr.indexOf("blue") == 0) {
           pixels.setPixelColor(led, pixels.Color(0, 0, 255)); //blue
        }
        else if (tStr.indexOf("black") == 0) {
          pixels.setPixelColor(led, pixels.Color(0, 0, 0)); //black
        }
        else if (tStr.indexOf("white") == 0) {
          pixels.setPixelColor(led, pixels.Color(255, 255, 255)); //white
        }
        else if (tStr.indexOf("random") == 0) {
          int a = random (0, 256);
          int b = random (0, 256);
          int c = random (0, 256);
          pixels.setPixelColor(led, pixels.Color(a, b, c));
        }
        else if (tStr.indexOf("semi") == 0) {
          switch (random (0, 8) ) {
            case 0:
              pixels.setPixelColor(led, pixels.Color(255, 0, 0)); //red
              break;
            case 1:
              pixels.setPixelColor(led, pixels.Color(0, 255, 0)); //green
              break;
            case 2:
              pixels.setPixelColor(led, pixels.Color(0, 0 , 255)); //blue
              break;
            case 3:
              pixels.setPixelColor(led, pixels.Color(255, 255, 0)); //red green
              break;
            case 4:
              pixels.setPixelColor(led, pixels.Color(255, 0, 255)); //red blue
              break;
            case 5:
              pixels.setPixelColor(led, pixels.Color(0, 255, 255)); //green blue
              break;
            case 6:
              pixels.setPixelColor(led, pixels.Color(255, 255, 255)); //white
              break;
            case 7:
              pixels.setPixelColor(led, pixels.Color(0, 0, 0)); //black
              break;
          }
        }
        else if (tStr.indexOf("help") == 0) {
          Serial.println("red green blue black white random semi");
        }

        //tStr = "";
      }

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayVal); // Delay for a period of time (in milliseconds).
  }
  }
*/
