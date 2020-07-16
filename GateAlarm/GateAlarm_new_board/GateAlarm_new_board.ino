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
//prototypes
void setup();
void loop();
String displayTime(int mode);
void serialRead();
void readThePins();
void extra();
void door_button_tone();
void gate_button_tone();
void gate_sensor_tone();
void gate_sensor_tone();
void cameraSwitch();
void tonePlayer(int Tone);
byte decToBcd(byte val);
byte bcdToDec(byte val);
void setDS3231time(byte second,
                   byte minute,
                   byte hour,
                   byte dayOfWeek,
                   byte dayOfMonth,
                   byte month,
                   byte year);

void readDS3231time(byte * second,
                    byte * minute,
                    byte * hour,
                    byte * dayOfWeek,
                    byte * dayOfMonth,
                    byte * month,
                    byte * year);
String displayTime(byte mode);

void printDebug();

//-----------------------------
//put your setup code here, to run once:
void setup() {  //The following is for the comm port
  Serial.begin (9600);
  Wire.begin();
  lcd.begin (20, 4); //LCD is 16x4
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  lcd.print("SainSmartI2C16x4");
  delay(2000);
  lcd.clear();
  lcd.print("Gate alarm v2.26");
  lcd.setCursor(0, 2);
  lcd.print(displayTime(1));

  //These are for the various IO pins
  pinMode(timer1, INPUT);
  pinMode(timer2, INPUT);
  pinMode(gate_button, INPUT);
  pinMode(gate_sensor, INPUT);
  pinMode(door_button, INPUT);
  pinMode(extra_sensor, INPUT);
  pinMode(tone_out, OUTPUT);
  pinMode(camera_out, OUTPUT);
  pinMode(horn_out, OUTPUT);
  digitalWrite(tone_out, LOW);
  digitalWrite(camera_out, LOW);
  digitalWrite(horn_out, LOW);
  Serial.println(displayTime(0));
}
//-----------------------------
//put your main code here, to run repeatedly:
void loop() {
  printDebug();
  serialRead();
  readThePins();
  //Serial.print(displayTime(0) + "\n"); // display the real-time clock data on the Serial Monitor,
}
//-----------------------------
//This handles input from user mostly debug stuff
void serialRead() {
  String tStr =  Serial.readString();
  
  if (tStr.length() > 1) {
    Serial.print(tStr  + ' '); //don't add an extra newline
    if (tStr.indexOf("gateB") == 0) {
      gate_button_tone();
    }
    else if (tStr.indexOf("gateS") == 0) {
      gate_sensor_tone();
    }
    else if (tStr.indexOf("door") == 0) {
      door_button_tone();
    }
    else if (tStr.indexOf("extra") == 0) {
      extra();
    }
    else if (tStr.indexOf("tone") == 0) {
      tone(tone_out, 500, 1500);
    }
    else if (tStr.indexOf("time0") == 0) {
      Serial.println(displayTime(0));
    }
    else if (tStr.indexOf("time1") == 0) {
      Serial.println(displayTime(1));
    }
    else if (tStr.indexOf("set") == 0) {
      // set the initial time here:
      Serial.println("seconds, minutes, hours, dow, day, month, year");
      Serial.println(tStr);

      #define MY_SIZE 16
      int myInts[MY_SIZE];
      int count=0;
      for (int i = 0; i <  int(tStr.length()); i++) {
        if (tStr.charAt(i) == ',') {
           myInts[count] = i;
           Serial.println(i);
           Serial.println(myInts[i]);
        }
      }
      
      Serial.println("");
      for(int i = 0; i < MY_SIZE; i++)
        {
          Serial.println(myInts[i]);
        }
      
      /*
 
       *  set 1,8,9,5,3
      byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
      Serial.println(second);
      Serial.println(minute); 
      Serial.println(hour);  
      Serial.println(dayOfWeek);  
      Serial.println(dayOfMonth);
      Serial.println(month);
      Serial.println(year); 
      */
   
      setDS3231time( 01, 35, 17, 01, 28, 05, 19);
      // void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

    }
    else if (tStr.indexOf("help") == 0) {
      Serial.println("gateB, gateS, door, extra, tone, set, time0, time1, help");
    };
    tStr = "";
  }
}
//-----------------------------
//Get the values of the input pins
void readThePins() {
  //timer1_val = analogRead(timer1);
  //timer2_val = analogRead(timer2);

  gate_sensor_val = analogRead(gate_sensor);
  if (gate_sensor_val > 450)  {
    gate_sensor_tone();
  }
  gate_button_val = analogRead(gate_button);
  if (gate_button_val > 450)  {
    gate_button_tone();
  }
  door_button_val = analogRead(door_button);
  if (door_button_val > 450)  {
    door_button_tone();
  }
  extra_sensor_val = analogRead(extra_sensor);
  if (extra_sensor_val > 450) {
    extra();
  }
}
//-----------------------------
void extra() {
  tonePlayer(1);
  Serial.println("extra");
  lcd.setCursor (0, 1);
  lcd.print("Extra           ");
  lcd.setCursor (0, 2);
  lcd.print(displayTime(1));
}
//-----------------------------
void door_button_tone() {
  static unsigned long BlockIt;
  if (BlockIt > millis() ) {
    tonePlayer(0);
    Serial.println("Door button skipped due to BlockIt");
    lcd.setCursor (0, 3);
    lcd.print("Door button skip");
    return;
  }
  tonePlayer(2);
  lcd.setCursor (0, 1);
  lcd.print("Door button      ");
  cameraSwitch();
  Serial.println("Door button");
  lcd.setCursor(0, 2);
  lcd.print(displayTime(1));
  BlockIt = millis() + 2e4; //20 seconds
}
//-----------------------------
void gate_button_tone() {
  static unsigned long BlockIt;
  if (BlockIt > millis() ) {
    tonePlayer(0);
    Serial.println("Gate button skipped due to BlockIt");
    lcd.setCursor (0, 3);
    lcd.print("Gate button skip");
    return;
  }
  tonePlayer(3);
  lcd.setCursor (0, 1);
  lcd.print("Gate button      ");
  cameraSwitch();
  Serial.println("Gate button");
  lcd.setCursor(0, 2);
  lcd.print(displayTime(1));
  BlockIt = millis() + 2e4; //20 seconds
}
//-----------------------------
void gate_sensor_tone() {
  static unsigned long BlockIt;
  if (BlockIt > millis() ) {
    tonePlayer(0);
    Serial.println("Gate sensor skipped due to BlockIt");
    lcd.setCursor (0, 3);
    lcd.print("Gate sensor skip");
    return;
  }
  tonePlayer(4);
  lcd.setCursor (0, 1);
  lcd.print("Gate sensor      ");
  cameraSwitch() ;
  Serial.println("Gate sensor");
  lcd.setCursor(0, 2);
  lcd.print(displayTime(1));
  BlockIt = millis() + 2e4; //20 seconds

}
//-----------------------------
void cameraSwitch() {
  digitalWrite(camera_out, HIGH);
  Serial.println("cameraSwitch");
  delay(3000);
  digitalWrite(camera_out, LOW);
}
//-----------------------------
void tonePlayer(int Tone) {
  int myTones[10];
  int myTones0[] = {400, 800, 0};
  int myTones1[] = {100, 200, 400, 600, 800, 1000, 1200, 0};
  int myTones2[] = {1200, 1000, 800, 600, 400, 200, 100, 0};
  int myTones3[] = {1200, 1000, 800, 600, 800, 1000, 1200, 0};
  int myTones4[] = {200, 400, 600, 800, 600, 400, 200, 0};
  int x;
  for (x = 0; x < 100; x++) {
    if (Tone == 0) {
      myTones[x] = myTones0[x];
      if (myTones0[x] == 0 ) {
        break;
      };
    };
    if (Tone == 1) {
      myTones[x] = myTones1[x];
      if (myTones1[x] == 0 ) {
        break;
      };
    };
    if (Tone == 2) {
      myTones[x] = myTones2[x];
      if (myTones2[x] == 0 ) {
        break;
      };
    };
    if (Tone == 3) {
      myTones[x] = myTones3[x];
      if (myTones3[x] == 0 ) {
        break;
      };
    };
    if (Tone == 4) {
      myTones[x] = myTones4[x];
      if (myTones4[x] == 0 ) {
        break;
      };
    };
  }

  for (x = 0; x < 100; x++) {
    if (myTones[x] == 0 ) {
      break;
    };
    Serial.print(String(x, DEC) + " " + myTones[x]);
    tone(tone_out, myTones[x] );
    delay(200);
  }
  noTone(tone_out);
  Serial.println();
}
//-----------------------------
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

//-----------------------------
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

//-----------------------------
void setDS3231time(byte second, 
                   byte minute,
                   byte hour, 
                   byte dayOfWeek, 
                   byte dayOfMonth, 
                   byte month, 
                   byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
//-----------------------------
void readDS3231time(byte * second,
                    byte * minute,
                    byte * hour,
                    byte * dayOfWeek,
                    byte * dayOfMonth,
                    byte * month,
                    byte * year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

//-----------------------------
/*Return a string with time and date
  0 full display
  1 no seconds
*/
String displayTime(int mode)
{
  String Output;

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,  &year);
  // send it to the serial monitor

  Output = String(hour, DEC);
  Output += ":";

  // convert the byte variable to a decimal number when displayed
  if (minute < 10)
  {
    Output += "0";
  }
  Output += String(minute, DEC);

  if (mode == 0)
  {
    Output += ":";
    if (second < 10)
    {
      Output += "0";
    }
    Output += String(second, DEC);
  }
  Output += " ";

  Output += String(year, DEC) + "/";
  Output += String(month, DEC) + "/";
  Output += String(dayOfMonth, DEC);

  Output += " ";

  switch (dayOfWeek) {
    case 1:
      Output += "Sun";
      break;
    case 2:
      Output += "Mon";
      break;
    case 3:
      Output += "Tue";
      break;
    case 4:
      Output += "Wed";
      break;
    case 5:
      Output += "Thu";
      break;
    case 6:
      Output += "Fri";
      break;
    case 7:
      Output += "Sat";
      break;
  }
  return (Output);
}

//-----------------------------
//Build a debug status string and print it if it has changed
void printDebug() {
  static String Saveit;

   char buffer [50];
   sprintf(buffer, "%03d  %03d  %03d  %03d", (door_button_val / 50) * 50,
                                             (extra_sensor_val / 50) * 50,
                                             (gate_button_val / 50) * 50,
                                             (gate_sensor_val / 50) * 50);

  if (Saveit != buffer) {
    Saveit = buffer;
    Serial.println(buffer);
  }
}
//-----------------------------
