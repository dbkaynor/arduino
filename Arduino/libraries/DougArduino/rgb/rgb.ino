/*
Adafruit Arduino - Lesson 3. RGB LED
 https://learn.adafruit.com/downloads/pdf/adafruit-arduino-lesson-3-rgb-leds.pdf
 https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/arduino-sketch

   The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include <LiquidCrystal.h>
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int Delay = 2000;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop()
{
  setColor(255, 0, 0); // red
  delay(Delay);
  setColor(10, 0, 0); // red
  //Serial.print("red\n");
  delay(Delay);
  setColor(0, 255, 0); // green
  //Serial.print("green\n");
  delay(Delay);
  setColor(0, 10, 0); // green
  delay(Delay);
  setColor(0, 0, 255); // blue
  //Serial.print("blue\n");
  delay(Delay);
  setColor(0, 0, 10); // blue
  delay(Delay);
  setColor(255, 255, 0); // yellow
  //Serial.print("yellow\n");
  delay(Delay);
  setColor(80, 0, 80); // purple
  //Serial.print("purple\n");
  delay(Delay);
  setColor(0, 255, 255); // aqua
  //Serial.print("aqua\n");
  delay(Delay);
}

void setColor(int red, int green, int blue)
{
#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

