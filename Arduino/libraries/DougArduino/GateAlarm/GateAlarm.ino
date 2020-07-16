#include <SD.h>
//#include <TimeLib.h>
//#include <Time.h>
#include <SoftwareSerial.h>

#define timer1 0         // Analog input A0
#define timer2 1         // Analog input A1
#define gate_button 2    // Analog input A2
#define gate_sensor 3    // Analog input A3
#define door_button 4    // Analog input A4
#define extra_sensor 5   // Analog input A5

#define camera_out 6     // Digital output D6 
#define horn_out 7       // Digital output D7
#define song_out 8       // Digital output D8
#define dingdong_out 9   // Digital output D9
#define bird_out 10      // Digital output D10
#define rxPin 11         // comm input  D11
#define txPin 12         // comm output D12

int timer1_val = 0;
int timer2_val = 0;

int gate_sensor_val = 0;
int gate_button_val = 0;
int door_button_val = 0;
int extra_sensor_val = 0;

// put your setup code here, to run once:
void setup() {
  //The following is for the comm port
  Serial.begin (9600);

  //These are for the various IO pins
  pinMode(timer1, INPUT);
  pinMode(timer2, INPUT);
  pinMode(gate_button, INPUT);
  pinMode(gate_sensor, INPUT);
  pinMode(door_button, INPUT);
  pinMode(extra_sensor, INPUT);
  defaultSettings();
}

// put your main code here, to run repeatedly:
void loop() {
  printDebug();
  readThePins();
  if (gate_sensor_val > 450)  {
    playBird();
  }
  if (gate_button_val > 450)  {
    playDingDong();
  }
  if (door_button_val > 450)  {
    playSong();
  }
  if (extra_sensor_val > 450) {
    playExtra();
  }

//This handles input from user mostly debug stuff
  String tStr =  Serial.readString();
  if (tStr.length() > 1) {
    Serial.print(tStr); //don't add an extra newline
    if (tStr.indexOf("bird") == 0) {
      playBird();
    }
    else if (tStr.indexOf("ding") == 0) {
      playDingDong();
    }
    else if (tStr.indexOf("song") == 0) {
      playSong();
    }
    else if (tStr.indexOf("extra") == 0) {
      playExtra();
    }
    else if (tStr.indexOf("help") == 0) {
      Serial.println("bird, ding, song, extra, help");
    };
    tStr = "";
  }

}

//Get the values of the input pins
void readThePins() {
  timer1_val = analogRead(timer1);
  timer2_val = analogRead(timer2);

  gate_sensor_val = analogRead(gate_sensor);
  gate_button_val = analogRead(gate_button);
  door_button_val = analogRead(door_button);
  extra_sensor_val = analogRead(extra_sensor);
}

void playDingDong() {
  static unsigned long BlockIt;
  Serial.println(String(BlockIt, DEC) + ' ' + String(millis(), DEC) );
  if (BlockIt > millis() ) {
    Serial.println("DingDong skipped due to BlockIt");
    return;
  }
  String tStr = "dingdong - gate button";
  Serial.println(tStr);

  pinMode(bird_out, INPUT);
  pinMode(dingdong_out, OUTPUT);
  digitalWrite(dingdong_out, LOW);
  delay(3000);
  BlockIt = millis() + 5e3; //5 seconds
  defaultSettings();
}

void playBird() {
  static unsigned long BlockIt;
  Serial.println(String(BlockIt, DEC) + ' ' + String(millis(), DEC) );
  if (BlockIt > millis() ) {
    Serial.println("Bird skipped due to BlockIt");
    return;
  }
  String tStr = "bird - gate sensor";
  Serial.println(tStr);

  pinMode(bird_out, OUTPUT);
  digitalWrite(bird_out, HIGH);
  delay(2000);
  BlockIt = millis() + 2e4; //20 seconds
  defaultSettings();
}

void playSong() {
  static unsigned long BlockIt;
  Serial.println(String(BlockIt, DEC) + ' ' + String(millis(), DEC) );
  if (BlockIt > millis() ) {
    Serial.println("Song skipped due to BlockIt");
    return;
  }
  String tStr = "song - door button";
  Serial.println(tStr);

  digitalWrite(song_out, HIGH);
  digitalWrite(horn_out, HIGH);
  delay(3000);
  digitalWrite(horn_out, LOW);
  BlockIt = millis() + 1e4; //10 seconds
  defaultSettings();
}

void playExtra() {
  static unsigned long BlockIt;
  Serial.println(String(BlockIt, DEC) + ' ' + String(millis(), DEC) );
  if (BlockIt > millis() ) {
    Serial.println("Extra skipped due to BlockIt");
    return;
  }
  String tStr = "extra sensor";
  Serial.println(tStr);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  BlockIt = millis() + 1e4; //10 seconds
  defaultSettings();
}

//Set all of pins to default state
void defaultSettings() {
  pinMode(dingdong_out, OUTPUT);  //D9
  pinMode(bird_out, OUTPUT);      //D10
  pinMode(song_out, OUTPUT);      //D11
  pinMode(horn_out, OUTPUT);      //D12

  digitalWrite(dingdong_out, HIGH);
  digitalWrite(bird_out, LOW);
  digitalWrite(song_out, LOW);
  digitalWrite(horn_out, LOW);
}

//Build a debug status string and print it if it has changed
void printDebug() {
  static String Saveit;

  String tStr =
    "  GSV:" + String(gate_sensor_val , DEC) +
    "  GBV:" + String(gate_button_val, DEC) +
    "  DBV:" + String(door_button_val, DEC) +
    "  ESV:" + String(extra_sensor_val, DEC);

  if (Saveit != tStr) {
    Saveit = tStr;
    Serial.println(tStr);
  }
}

