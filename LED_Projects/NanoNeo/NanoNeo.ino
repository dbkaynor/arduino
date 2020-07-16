#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>
/*
    Board: Arduino Nano
    Processor: ATmega328P (Old Bootloader)
    QinHeng Electronics HL-340 USB-Serial adapter
*/

/* changed from #defined constants to variables 2015-12-15 Mel Lester Jr.
  The following two lines are all you may need to change for your project */
int dataPin = 6;  // Arduino PWM data pin D6
int NUM_LEDS = 3;  // number of Neopixels
#define NUM_LEDS 38
#define BRIGHTNESS 70

#define varResistor1 0     // Analog input A0
#define varResistor2 1     // Analog input A1
/*
  #define hexSwitch1 9       // hex switch definitions
  #define hexSwitch2 10      // hex switch definitions
  #define hexSwitch4 11      // hex switch definitions
  #define hexSwitch8 12      // hex switch definitions
  int hexS1 = 0;
  int hexS2 = 0;
  int hexS4 = 0;
  int hexS8 = 0;
  int hexVal = 0;
*/

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

int loop_count = 0;
//-------------------------------------------------------
void setup() {
  printDebug("setup");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin (9600); //The following is for the comm port
}
//-------------------------------------------------------
void loop() {
  loop_count++;
  printDebug("---- Loop " + String(loop_count) + "----");
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  pulseWhite(50);
  Colors64K(50);
}
//-------------------------------------------------------
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  printDebug("colorWipe");
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
  printDebug("Wheel");
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

//-------------------------------------------------------
//Build a debug status string and print it if it has changed
void printDebug(String trace) {
  //static String Saveit;

  //if (Saveit != trace) {
   Serial.println(trace);
  //  Saveit = trace;
  //}
}
