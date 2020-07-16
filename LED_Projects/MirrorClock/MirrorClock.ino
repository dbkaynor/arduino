#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdlib.h>
/*
    Board: Arduino Nano
    Processor: ATmega328P (Old Bootloader)
    QinHeng Electronics HL-340 USB-Serial adapter
*/
int dataPin = 7;  // Arduino PWM data pin D6
#define NUM_LEDS 38  // number of Neopixels  38
#define BRIGHTNESS 10
#define WAIT 50
#define varResistor1 0     // Analog input A0
#define varResistor2 1     // Analog input A1

#define hexSwitch1 9       // hex switch definitions
#define hexSwitch2 10      // hex switch definitions
#define hexSwitch4 11      // hex switch definitions
#define hexSwitch8 12      // hex switch definitions

int delayVal1 = 500;
int delayVal2 = 0;

int hexS1 = 0;
int hexS2 = 0;
int hexS4 = 0;
int hexS8 = 0;
int hexVal = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, dataPin, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

word loop_count = 0;
//-------------------------------------------------------
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(BRIGHTNESS);
  Serial.begin(9600); //The following is for the comm port
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
  //Serial.println(trace + "    " + loop_count);

  if (Saveit != trace) {
    Serial.println(trace + "    " + loop_count);
    Saveit = trace;
  }
}
//-----------------------------
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
//This function reads the variable resistors and hex switch
void callback()
{
  delayVal1 = analogRead(varResistor1);
  delayVal2 = analogRead(varResistor2);

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

  int di = 5;
  String ts = (String((delayVal1 / di) * di) + "  " + String((delayVal2 / di) * di) +
               " [" + String(hexVal, BIN) + "  " + String(hexVal, HEX) + "  " + String(hexVal, DEC) + "]");

  printDebug(ts);
}

//-------------------------------------------------------
void loop() {
  strip.setBrightness(BRIGHTNESS);
  uint32_t delay_value = 1000;
  loop_count++;
  printDebug("---- Loop " + String(loop_count) + "----");
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50, "Red"); // Red
  delay(delay_value);
  colorWipe(strip.Color(0, 255, 0), 50, "Green");
  delay(delay_value);
  colorWipe(strip.Color(0, 0, 255), 50, "Blue");
  delay(delay_value);
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127,   0,   0), 50); // Red
  //theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  pulseWhite(50);
  Colors64K(50);
}
//-------------------------------------------------------
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, const char* input) {

  char src[50], dest[50];

  strcpy(src, "Colorwipe: ");
  strcat(src, input);
  printDebug(src);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
//-------------------------------------------------------
void rainbow(uint8_t wait) {
  printDebug("rainbow");
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
//-------------------------------------------------------
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  printDebug("rainbowCycle");
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
//-------------------------------------------------------
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  printDebug("theaterChase");
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}
//-------------------------------------------------------
//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  printDebug("theaterChaseRainbow");
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}
//-------------------------------------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  //  printDebug("Wheel");
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//-------------------------------------------------------
void Colors64K(uint8_t delayValue) {
  printDebug("Colors64k");
  for (int led = 0; led < NUM_LEDS  ; led++) { // Counts through the neopixels
    int a = random (0, 256);
    int b = random (0, 256);
    int c = random (0, 256);
    strip.setPixelColor(led, strip.Color(a, b, c));
  }
  strip.show(); // This sends the updated pixel color to the hardware.
  delay(delayValue);
}
//-------------------------------------------------------
void pulseWhite(uint8_t delayValue) {
  printDebug("pulseWhite");
  for (int j = 0; j < 255 ; j++) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }

  for (int j = 255; j >= 0 ; j--) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
  }
}
