/* Mikaela Diaz
 * 12/03/23
 * Description: DF Robot SIM808 with ESP32 code to send latitude and longitude to ThingSpeak. 
 * It also generates a google maps link with the location coordinates.
 
  WriteMultipleFields
  Description: Writes values to fields 1,2 and status in a single ThingSpeak update every 20 seconds.
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  For licensing information, see the accompanying license file. 
  Copyright 2020, The MathWorks, Inc.
*/

#include <DFRobot_SIM808.h>
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

DFRobot_SIM808 sim808(&Serial2);
const int PWR_PIN = 23;

float lat = 0, lon = 0;
String sms = "My location is ";
String smsFinal;
char smsC[40];
String mapsLocation = "https://maps.google.com/?q=";

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myStatus = "";

void setup() {

  Serial2.begin(9600);//Rx tx
  
  Serial.begin(115200);  //Initialize serial
  pinMode (PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN,LOW);
  delay(1000);
  digitalWrite(PWR_PIN,HIGH);
  delay(2000);
  digitalWrite(PWR_PIN,LOW);
  delay(3000);
  //******** Initialize sim808 module *************
  while (!sim808.init()) {
    delay(1000);
    Serial.print("Sim808 init error\r\n");
  }

  //************* Turn on the GPS power************
  if ( sim808.attachGPS())
    Serial.println("Open the GPS power success");
  else
    Serial.println("Open the GPS power failure");
  
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo native USB port only
  //}
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  //************** Get GPS data *******************
  if (sim808.getGPS()) {
    Serial.println();
    Serial.print(sim808.GPSdata.year);
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);

    Serial.print("\nLatitude :");
    Serial.println(sim808.GPSdata.lat, 6);
    lat = sim808.GPSdata.lat;

    Serial.print("Longitude :");
    Serial.println(sim808.GPSdata.lon, 6);
    lon = -sim808.GPSdata.lon; // add minus sign as we are west of GMT

    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);

    //************* Turn off the GPS power ************
    // sim808.detachGPS();

    // set the fields with the values
    ThingSpeak.setField(1, lat);
    ThingSpeak.setField(2, lon);
  
    // set the status
    ThingSpeak.setStatus(myStatus);
    
    // write to the ThingSpeak channel (all data together)
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    } 

    Serial.println("Map is getting ready ...");
    String latCPPStr = String(lat); //convert float to string
    mapsLocation += latCPPStr;
    latCPPStr += "Lat";
    String lonCPPStr = String(lon);
    mapsLocation += "," + lonCPPStr;
    lonCPPStr += "Lon";
    smsFinal = sms + latCPPStr + " " + lonCPPStr;
    smsFinal.toCharArray(smsC, smsFinal.length() + 1); // convert String to C String
    Serial.println(smsC);
    Serial.println(mapsLocation);
    mapsLocation = "https://maps.google.com/?q=";
    
    delay(20000); // Wait 20 seconds to update the channel again (Can send all readings at a time)
  }
}
