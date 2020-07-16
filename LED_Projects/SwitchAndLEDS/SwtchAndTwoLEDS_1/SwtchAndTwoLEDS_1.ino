
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>

#include <Adafruit_NeoPixel.h>

/*

*/

#define PIN           4  // Output pin to NeoPixels
#define NUMPIXELS     4  // Number of NeoPixels
#define SWITCH        2  // Control switch pin

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

int delayval = 250; // delay time in milli seconds

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(SWITCH , INPUT_PULLUP);
  Serial.begin (9600); //The following is for the comm port
}

void loop() {
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
      delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

//Build a debug status string and print it if it has changed
void printDebug() {
  static String Saveit;
  String tStr;


  if (Saveit != tStr) {
    Saveit = tStr;
    Serial.println(tStr);
  }
}
