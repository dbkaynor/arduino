/*
  Rotary Encoder - get the position
  modified on 23 Feb 2019
  by Saeed Hosseini
  https://electropeak.com/learn/
*/

#define encoderOutA 6 // CLK
#define encoderOutB 7 // DT
#define button 8 // SW
#define led 13 // On board orange led

int counter = 0;
int State;
int old_State;

void setup() {
  pinMode (encoderOutA, INPUT);
  pinMode (encoderOutB, INPUT);
  pinMode (button, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin (9600);
  //Read First Position of Channel A
  old_State = digitalRead(encoderOutA);
}

void loop() {
  if (digitalRead(button) == 0) {
    old_State = 0;
    State = 0;
    counter = 0;
    digitalWrite(led, HIGH);
  }
  State = digitalRead(encoderOutA);
  if (State != old_State)
  {
    digitalWrite(led, LOW);
    if (digitalRead(encoderOutB) != State)
    {
      counter ++;
    }
    else {
      counter --;
    }
    Serial.print("Position: ");
    Serial.println(counter);

  }
  old_State = State; // the first position was changed
}
