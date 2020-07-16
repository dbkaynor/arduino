//A sketch to demonstrate the tone() function
#include <SoftwareSerial.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <LCD.h>
//Specify digital pin on the Arduino that the positive lead of piezo buzzer is attached.
#define tonePin  2
#define blue 2
#define orange 3


void setup() {
  pinMode(orange,OUTPUT);
  pinMode(blue,OUTPUT);

}//close setup

void loop() {
  digitalWrite(orange, HIGH);
  tone(tonePin, 1000, 500);
  delay(1500);
  digitalWrite(orange, LOW);
  digitalWrite(blue,HIGH);
  tone(tonePin, 500, 500);
  delay(1500);
  digitalWrite(blue,LOW);

}
