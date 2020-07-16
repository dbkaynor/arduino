
#include <SoftwareSerial.h>
/*
    Board: Arduino Nano
    Processor: ATmega328P (Old Bootloader)
    QinHeng Electronics HL-340 USB-Serial adapter
*/
#define tone_out 2       //Digital ouput  D2
int myTones[] = {100, 200, 400, 600, 800, 1000, 1200, 0};
void setup()
{
  Serial.begin (9600);
  Serial.println("Hello");
};

void loop()
{
int x;
for (x=0; x<100; x++) {
  
   if (myTones[x] == 0 ) {break;};
   Serial.println(String(x,DEC) + " " + myTones[x]);
   tone(tone_out,myTones[x] );
   delay(100);
}
