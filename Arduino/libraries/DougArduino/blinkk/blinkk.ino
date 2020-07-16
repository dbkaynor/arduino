

void setup() {
  pinMode(13, OUTPUT); //set pin 13 as output
}
void loop() {
  digitalWrite(13, HIGH); //set pin 13 high (+5V)
  delay(4000); //wait 1000 ms = 1 second
  digitalWrite(13, LOW); //set pin 13 low (0V)
  delay(400); // wait 1 second
}
