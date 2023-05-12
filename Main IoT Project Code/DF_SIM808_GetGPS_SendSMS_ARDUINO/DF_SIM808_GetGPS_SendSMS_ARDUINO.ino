/* Mikaela Diaz
 * 11/03/23
 * Description: Code to send an SMS text containing the GPS coordinates using an Arduino Uno. The code uses C++ strings.
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [Jason](jason.ling@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-02-08
 * @url  https://github.com/DFRobot/DFRobot_SIM808
 */
#include <DFRobot_SIM808.h>

// #define CONNECT_BY_JUMPER   1
#if CONNECT_BY_JUMPER
  #define PIN_TX    10
  #define PIN_RX    11
  SoftwareSerial mySerial(PIN_TX, PIN_RX);
  DFRobot_SIM808 sim808(&mySerial);
/**
 * Use Leonardo for push-in connection
 * Set DIP switch to 3-Arduino, and use the Serial1 of Leonardo for communication
 */
#elif defined(ARDUINO_AVR_LEONARDO)
  DFRobot_SIM808 sim808(&Serial1);
/**
 * Use UNO & MEGA2560 for push-in connection
 * Set DIP switch to 3-Arduino, and use the Serial of UNO and MEGA2560 for communication
 */
#else
  DFRobot_SIM808 sim808(&Serial);
#endif

//Mobile phone number, replace with a phone number
#define PHONE_NUMBER  ""  

float lat = 0, lon = 0;
String sms = "My location is ";
String smsFinal;
char smsC[40];

void setup() {
  #if CONNECT_BY_JUMPER
    mySerial.begin(9600);
  #elif defined(ARDUINO_AVR_LEONARDO)
    Serial1.begin(9600);
  #endif
  Serial.begin(9600);

  //******** Initialize sim808 module *************
  while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  } 
  Serial.println("Sim808 init success");

  //************* Turn on the GPS power************
  if( sim808.attachGPS()) 
      Serial.println("\nOpen the GPS power success");
  else 
      Serial.println("Open the GPS power failure");
}

void loop() {
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
    
    Serial.print("\nLatitude : ");
    Serial.println(sim808.GPSdata.lat,6);
    lat = sim808.GPSdata.lat;
    
    Serial.print("Longitude : -");
    Serial.println(sim808.GPSdata.lon,6);
    lon = sim808.GPSdata.lon;
    
    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);

    //************* Turn off the GPS power ************
    sim808.detachGPS();

    Serial.println("Start to send message ...");
    String latCPPStr = String(lat); //convert float to string
    latCPPStr += "Lat";
    String lonCPPStr = String(lon);
    lonCPPStr += "Lon";
    smsFinal = sms + latCPPStr + " " + "-" + lonCPPStr;
    smsFinal.toCharArray(smsC, smsFinal.length() + 1); // convert String to C String
    
    //******** define phone number and text **********
    sim808.sendSMS((char *)PHONE_NUMBER,smsC); 
    Serial.println("\nMessage sent.");
  }
}
