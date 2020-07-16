#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>
/*
  Board: Arduino Nano
  Processor: ATmega328P (Old Bootloader)
  QinHeng Electronics HL-340 USB-Serial adapter
*/
/*
  https://www.arduino.cc/en/Tutorial/AnalogInputPins
  https://www.arduino.cc/en/Reference/HomePage
  https://www.sparkfun.com/products/9568
  https://www.arduino.cc/en/Reference/softwareSerial
*/
#define DS3231_I2C_ADDRESS 0x68
#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner

#define BACKLIGHT_PIN 3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

#define door_button  1    //Analog input A0
#define extra_sensor 0    //Analog input A1
#define gate_button  3    //Analog input A2
#define gate_sensor  2    //Analog input A3
// A4 and A5 are SDA and SCL for LCD, and other i2c devices  //0x27

#define timer1 6         //Analog input A6
#define timer2 7         //Analog input A7

#define tone_out 2       //Digital ouput  D2
#define camera_out 6     //Digital output D6
#define horn_out 7       //Digital output D7
//#define rxPin 11       //comm input  D11
//#define txPin 12       //comm output D12
//-----------------------------
int timer1_val = 0;
int timer2_val = 0;

int gate_sensor_val = 0;
int gate_button_val = 0;
int door_button_val = 0;
int extra_sensor_val = 0;
//-----------------------------
//put your setup code here, to run once:
void setup() {  //The following is for the comm port
  Serial.begin (9600);
  //These are for the various I/O pins
  pinMode(timer1, INPUT);
  pinMode(timer2, INPUT);
  pinMode(gate_button, INPUT);
  pinMode(gate_sensor, INPUT);
  pinMode(door_button, INPUT);
  pinMode(extra_sensor, INPUT);
  pinMode(tone_out, OUTPUT);
  pinMode(camera_out, OUTPUT);  // green/white cable
  pinMode(horn_out, OUTPUT);
  digitalWrite(tone_out, LOW);
  digitalWrite(camera_out, LOW);
  digitalWrite(horn_out, LOW);

  Serial.begin (9600);
  Wire.begin();
  lcd.begin (20, 4); //LCD is 16x4
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home(); // go home
  lcd.print("SainSmartI2C16x4");
  lcd.setCursor(0, 1);
  lcd.print("Gate alarm v2.26");
  lcd.setCursor(0, 2);
  lcd.print("Gate Alarm Test");
}
//-----------------------------

//This handles input from user mostly debug stuff
void serialRead() {
  String tStr =  Serial.readString();
  if (tStr.length() > 1) {
    Serial.print(tStr + ' '); //don't add an extra newline
    lcd.setCursor(0, 3);
    lcd.print(tStr);
  }
}
//-----------------------------
//Get the values of the input pins
void readThePins() {
  gate_sensor_val = analogRead(gate_sensor);
  gate_button_val = analogRead(gate_button);
  door_button_val = analogRead(door_button);
  extra_sensor_val = analogRead(extra_sensor);
}
//-----------------------------
//re, to run repeatedly:
//put your main code here
void loop() {
  //serialRead();
  //Serial.print(displayTime(0) + "\n"); // display the real-time clock data on the Serial Monitor,
  readThePins();
  printDebug();
  /*
  //int timer = 500;
  //digitalWrite(horn_out, HIGH);
  readThePins();
  printDebug();
  delay(timer);
  digitalWrite(horn_out, LOW);
  delay(timer);
  readThePins();
  printDebug();
  digitalWrite(camera_out, HIGH);
  readThePins();
  printDebug();
  delay(timer);
  digitalWrite(camera_out, LOW);
  delay(timer);
  readThePins();
  printDebug();

  digitalWrite(tone_out, HIGH);
  readThePins();
  printDebug();
  delay(timer);
  digitalWrite(tone_out, LOW);
  delay(timer);
  readThePins();
  printDebug();
  */
}
//-----------------------------
//Build a debug status string and print it if it has changed
void printDebug() {
  static String Saveit;

   char buffer [50];
   sprintf(buffer, "%s%03d  %s%03d  %s%03d  %s%03d", 
                   "door:", (door_button_val / 50) * 50,
                   "extra:", (extra_sensor_val / 50) * 50,
                   "gateB:", (gate_button_val / 50) * 50,
                   "gateS:", (gate_sensor_val / 50) * 50);

  if (strcmp(buffer, "door:000  extra:000  gateB:000  gateS:000") == 0) {
    return;
  }
  
  if (Saveit != buffer) {
    Saveit = buffer;
    Serial.println(buffer);
  }
}
