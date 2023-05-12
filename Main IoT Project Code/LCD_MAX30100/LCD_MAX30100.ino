/* Mikaela Diaz
 * 12/02/23
 * Code to read Blood Oxygen and Heart Rate values from the MAX30100 sensor. The values are printed to 
 * the serial monitor.
 * It also used the library LiquidCrystal to print the readings to an LCD Display.
 * 
 * Code Sources:
  Arduino-MAX30100 oximetry / heart rate integrated sensor library
  Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>
  MAX30100_PulseOximete.h library (Minimal) example.
*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000 //define the reporting time in ms between the samples

const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
// Create lcd object (instance)
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //constructor is different than for arduino pins

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation

// Create an object pox of PulseOximeter 
PulseOximeter pox; 

// Variable to hold the time at which the last beat occurred
uint32_t tsLastReport = 0;

//Special heart character
byte heart0[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() { // The following callback function will display “Beat!” in the serial monitor when a heart beat occurs
  Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200); // ESP32 baud rate
  lcd.createChar(0, heart0); // create a new custom character (index 0)
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:

  // Print a message to the LCD.
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("Health-Tracking");
  lcd.setCursor(0, 1);
  lcd.print("Smartwatch");
  lcd.setCursor(15, 1); // move cursor to (15, 1)
  lcd.write((byte)0);  // print the custom char at (15, 1)
  delay(3000);

  // Initialize the PulseOximeter instance (Failures are generally due to an improper I2C wiring, missing power supply or wrong target chip)
  Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
      Serial.println("FAILED");
      for(;;);
  } else {
      Serial.println("SUCCESS");
  }
  // The default current for the IR LED is 50mA and it could be changed
  //   by uncommenting the following line. Check MAX30100_Registers.h for all the
  //   available options.
  // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected); // Function that calls anorher function
}

void loop() {
  // Read from the sensor
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BPM  : ");
    lcd.print(pox.getHeartRate());
    lcd.setCursor(13, 0); // move cursor to (13, 1)
    lcd.write((byte)0);  // print the custom char at (13, 0)

    lcd.setCursor(0, 1);
    lcd.print("SpO2 : ");
    lcd.print(pox.getSpO2());
    lcd.print(" %");

    tsLastReport = millis();
  } 
}
