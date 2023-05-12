/*
  Mikaela Diaz
  18/03/23
  Description: Code that calculates the distance of an object and gives off a warning in the form
  of a Buzzer, an LED, a print warning message and an LCD exclamation if the object is closer
  than 60 cms.
  
  Code source: Natasha's example. This code has been edited to fit my preferences.
  Ultrasonic sensor Pins:
      VCC: +5VDC
      Trig : Trigger (INPUT) - GPIO 5
      Echo: Echo (OUTPUT) - GPIO 18.
      This should be divided using a resistor divider to give a suitable 0 - 3V3 i/p range
      GND: GND
*/
#include "HC_SR04.h"

// Create lcd object (instance)
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //constructor is different than for arduino pins

void setup() {
  //Serial Port begin
  Serial.begin (115200);
  //Define inputs and outputs
  configHCSR_04Pins();
}

void loop() {
  float cms, inches;
  cms = measureDistanceCms();
  inches = measureDistanceInches();
  printInches(inches);
  printCms(cms);
  distanceWarning(cms);

  delay(500);
}
