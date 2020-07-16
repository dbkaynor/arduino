
// https://www.arduino.cc/en/Tutorial/AnalogInputPins
// https://www.arduino.cc/en/Reference/HomePage

//#include <LiquidCrystal.h>

//LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

#define timer1 0      // Analog input A0
#define timer2 1      // Analog input A1
#define gate_sensor 2 // Analog input 21
#define gate_button 3 // Analog input
#define door 4        // Analog input
#define package 5     // Analog input

int timer1_val = 0;
int timer2_val = 0;
int gate_sensor_val = 0;
int gate_button_val = 0;
int door_val = 0;
int package_val = 0;

// put your setup code here, to run once:
void setup() {
  Serial.begin (9600);
  pinMode(timer1, INPUT);
  pinMode(timer2, INPUT);
  pinMode(gate_button, INPUT_PULLUP);
  pinMode(gate_sensor, INPUT_PULLUP);
  pinMode(door, INPUT_PULLUP);
  pinMode(package, INPUT_PULLUP);
  //lcd.begin(16,1);
  //lcd.print("hello, world!");
}

// put your main code here, to run repeatedly:
void loop() {
  timer1_val = analogRead(timer1);
  timer2_val = analogRead(timer2);
  gate_sensor_val = analogRead(gate_sensor);
  gate_button_val = analogRead(gate_button);
  door_val = analogRead(door);
  package_val = analogRead(package);

  Serial.print(timer1_val);
  Serial.print("    ");
  Serial.print(timer2_val);
  Serial.print("  :  ");
  Serial.print(gate_sensor_val);
  Serial.print("    ");
  Serial.print(gate_button_val);
  Serial.print("    ");
  Serial.print(door_val);
  Serial.print("    ");
  Serial.println(package_val);
  delay(1000);
}
