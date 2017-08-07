/* 
 * Date: 2/15/16
 * Example Sleep Program using the Analog Compator
 * Originally based on Nick Gammon's Analog Sleep Comparator Program
 * Atmel328p processor with accelerometer connected to PD6 (AIN0), Digital Pin 6 on Arduino IDE
 * Pin PD7 (AIN1), Digital Pin 7 on Arduino IDE, has a voltage divider set to 1.5 V.
 *
*/

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>

int ledPin = 4;
volatile boolean triggered;

void setup(void)
{
  pinMode(ledPin,OUTPUT);   // set LED pin (D4) as an output
  digitalWrite(ledPin,LOW); // turn LED on (active low)

  ADCSRB = 0;       // (Disable) ACME: Analog Comparator Multiplexer Enable
  ACSR = bit (ACI)  // (Clear) Analog Comparator Interrupt Flag
   | bit (ACIE)     // Analog Comparator Interrupt Enable
   | bit (ACIS1);   // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on falling edge)
  triggered = 0;
}

void loop(void) {
  if (triggered == true) {
    digitalWrite(ledPin,LOW); // turn LED on (active low)
    delay(3000); // wait to see the LED before sleeping again
  }
  sleepNow();
}

void sleepNow(void) {
  triggered = false;
  digitalWrite(ledPin,HIGH);  // turn LED off (active low)
  // Choose our preferred sleep mode:
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  // Set sleep enable (SE) bit:
  sleep_enable();
  
  // Put the device to sleep:
  sleep_mode();

  // Upon waking up, sketch continues from this point.
  sleep_disable();
}

ISR (ANALOG_COMP_vect) {
  triggered = true;
}
