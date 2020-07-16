
/*
** Example Arduino sketch for SainSmart I2C LCD Screen 16x2
** based on https://bitbucket.org/celem/sainsmart-i2c-lcd/src/3adf8e0d2443/sainlcdtest.ino
** by
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)

** This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal 

** Modified - Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc

** Written for and tested with Arduino 1.0
**
** NOTE: Tested on Arduino Uno whose I2C pins are A4==SDA, A5==SCL
/*
    Board: Arduino Nano
    Processor: ATmega328P (Old Bootloader)
    QinHeng Electronics HL-340 USB-Serial adapter
    lcd_I2C is on address 0x27
*/

*/
#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//https://forum.arduino.cc/index.php?topic=128635.0
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

/*
- My LCD setup had PCF8574T chip as the 8-bit I/O expander for I2C bus. (Address 0x27).
- 8 pins from PCF8574T connect to the LCD display as follows (in this model of the board) :
    LCD <------> pcf8574T CHIP
     RS(pin4)    - P0 (pin4)
     RW(pin5)    - P1 (pin5)
     EN(pin6)    - P2 (pin6)
     BL(pin3)    - P3 (pin7)
     D4(pin11)   - P4 (pin9)
     D5(pin12)   - P5 (pin10)
     D6(pin13)   - P6 (pin11)
     D7(pin14)   - P7 (pin12)

So, for example, in the code:
#define En_pin  2
means EN
 */


int n = 1;

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup()
{
 lcd.begin (16,2); //  <<----- My LCD was 16x2

// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home (); // go home

 lcd.print("SainSmartI2C16x2");  
}

void loop()
{
 // Backlight on/off every 3 seconds
 lcd.setCursor (0,1);        // go to start of 2nd line
 lcd.print(n++,DEC);
 lcd.setBacklight(LOW);      // Backlight off
 delay(3000);
 lcd.setBacklight(HIGH);     // Backlight on
 delay(3000);
}
