
void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   delay(10);
   analogWrite(4,0);
   digitalWrite(A3, LOW);
   digitalWrite(A4, LOW);
   digitalWrite(A5, LOW);
   delay(10);
   analogWrite(4,255);
   digitalWrite(A3, HIGH);
   digitalWrite(A4, HIGH);
   digitalWrite(A5, HIGH);

}
