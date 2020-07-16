
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1

#define PIN           4  // Output pin to NeoPixels
#define NUMPIXELS     5  // Number of NeoPixels
#define BRIGHTNESS   70
#define SWITCH        2  // Control switch pin

const int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 250; // delay time in milli seconds

void setup() {
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
  pinMode(SWITCH, INPUT_PULLUP);
  Serial.begin (9600); //The following is for the comm port
  irrecv.enableIRIn(); // Start the receiver
  printDebug("Setup complete");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  for (int led = 0; led < NUMPIXELS ; led++) {
    if (digitalRead(SWITCH) == LOW ) {
      printDebug("Lots of colors");
      int a = random (0, 256);
      int b = random (0, 256);
      int c = random (0, 256);
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(led, pixels.Color(a, b, c));
    }
    else
    {
      printDebug("Eight colors");
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
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}
//Build a debug status string and print it if it has changed
void printDebug(String trace) {
  static String Saveit;
  String tStr = trace;
  if (Saveit != tStr) {
    Saveit = tStr;
    Serial.println(tStr);
  }
}
