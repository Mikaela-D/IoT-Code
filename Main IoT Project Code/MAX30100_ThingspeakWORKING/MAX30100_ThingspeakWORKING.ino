/*
  Mikaela Diaz
  1/03/23
  Code to read Blood Oxygen and Heart Rate values from the MAX30100 sensor and send them to Thingspeak.
  Source: MAX30100_PulseOximete.h library (Minimal) example.
  
  Arduino-MAX30100 oximetry / heart rate integrated sensor library
  Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>
*/

#include <WiFi.h>
#include <Wire.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     5000 //define the reporting time in ms between the samples

int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";  //note to myself, remember to send readings only if they are not 0

// Create an object pox of PulseOximeter 
PulseOximeter pox; 

// Variable to hold the time at which the last beat occurred
uint32_t tsLastReport = 0;

int count = 0;
float heartRate = 0;
float SpO2 = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected() {
  // The following callback function will display “Beat!” in the serial monitor when a heart beat occurs
  Serial.println("Beat!");
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("\n");

    WiFi.mode(WIFI_STA);  
    ThingSpeak.begin(client);  // Initialize ThingSpeak

    // Initialize the PulseOximeter instance (Failures are generally due to an improper I2C wiring, missing power supply or wrong target chip)
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    } 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected); // Function that calls anorher function
}

void loop() {
  // Read from the sensor
  pox.update();
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate : ");
    //Serial.print(pox.getHeartRate()); //method getHeartRate()
    heartRate = pox.getHeartRate();
    Serial.println(heartRate);
    
    Serial.print("bpm / SpO2:");
    //Serial.print(pox.getSpO2()); //method getSpO2()
    SpO2 = pox.getSpO2();
    Serial.print(SpO2);
    Serial.println("%");

    tsLastReport = millis();

    count++;
    if (count == 6) {
      count = 0;  
      Serial.println();
      Serial.println("Sending to Thingspeak");
      Serial.print("Heart rate : ");
      Serial.println(heartRate);
      Serial.print("bpm / SpO2:");
      Serial.print(SpO2);
      Serial.println("%");

       // set the fields with the values
      ThingSpeak.setField(1, heartRate);
      ThingSpeak.setField(2, SpO2); 
    
      // set the status
      ThingSpeak.setStatus(myStatus);
       // write to the ThingSpeak channel (all data together)
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);   
     
      if(x == 200) {
        Serial.println("Channel update successful.");
      }
      else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      pox.begin();
    }
  }
}
