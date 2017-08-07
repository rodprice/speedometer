/* speedometer.ino
   Rodney Price, May 2017
   Measure the velocity of a potato as it leaves the potato gun barrel.
*/


#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>

#include <LiquidCrystal.h>

#define LED 13

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

volatile boolean triggered;

ISR (ANALOG_COMP_vect) {
  triggered = true;
}

void setup () {
  pinMode(LED, OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  /* Serial.begin (115200); */
  /* Serial.println ("Started."); */
  ADCSRB = 0;           // (Disable) ACME: Analog Comparator Multiplexer Enable
  ACSR = bit (ACI)      // (Clear) Analog Comparator Interrupt Flag
    | bit (ACIE)        // Analog Comparator Interrupt Enable
    | bit (ACIS1);      // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on falling edge)
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(micros());
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  if (triggered) {
    Serial.println ("Triggered!");
    triggered = false;
  }
}

