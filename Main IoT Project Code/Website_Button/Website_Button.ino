/*  
  Mikaela Diaz
  3/04/23
  Description: Website code from an ESP32 server to display a button.
  When the button is pressed, a request is sent to get the GPS location of the SIM808.
  Then, the location is sent in an SMS text message in the form of a Google Maps link.
  Code Source: Example given by lecturer. The code has been edited to fit my website with the button and GPS 
  SMS code.
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_SIM808.h>
#include "homepage.h"

//Mobile phone number,need to change
#define PHONE_NUMBER  "" //number you are texting to

//The content of messages sent
#define MESSAGE  "hello,from ESP32"
const int PWR_PIN = 23; // if you want a SW reboot of Sim808

const char* ssid = ""; // Replace with your ssid
const char* password = ""; // Replace with your password

WiFiClient  client;
String myStatus = "";

String text = "Pos: ";
String textFinal;
char textC[40];// buffer to hold C string with long and lat coordinates not in GoogleMaps format
char mapsLocInC[40];// buffer to hold C string with location as Google Map link
String mapsLocation = "http://maps.google.com/?q=";

// Use Serial2 for ESP32
DFRobot_SIM808 sim808(&Serial2);

// Set web server port number to 80
WebServer server(80);

void handleRoot() {
  String message = mainPage;
  server.send(200, "text/html", message);
}

void sendSMS_GPS() {
  Serial.println("\nButton pressed");
  float lat, lon;
  while (!sim808.getGPS()); //wait for a GPS location
  Serial.print("Latitude :");
  lat = sim808.GPSdata.lat;
  Serial.println(lat, 6);
  lon = -sim808.GPSdata.lon; // add minus sign as we are west of GMT
  Serial.print("Longitude :");
  Serial.println(lon, 6);

  String latCPPStr = String (lat);// conv float to String
  mapsLocation += latCPPStr;
  latCPPStr += "Lat";
  String lonCPPStr = String(lon);
  mapsLocation += "," + lonCPPStr;
  lonCPPStr += "Lon";
  textFinal = text + latCPPStr + " " + lonCPPStr;

  textFinal.toCharArray(textC, textFinal.length() + 1);//conv String to C string
  Serial.println (textC);// textC can be sent to send long and lat location in text not in GoogleMap format
  Serial.println (mapsLocation);
  // Send text with GoogleMaps location
  mapsLocation.toCharArray(mapsLocInC, mapsLocation.length() + 1);//conv String to C string
  sim808.sendSMS((char *)PHONE_NUMBER, mapsLocInC);
  mapsLocation = "http://maps.google.com/?q="; // reset if you want to read another location

  while (1);//nothing to do. Don't want to keep sending texts and wasting credit!

  // Send to server 
  server.send(200);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial2.begin(9600); // RX TX
  Serial.begin(115200);
  pinMode (PWR_PIN, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/sentKeyPressToWebServer", sendSMS_GPS);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin(); // Initialise server
  Serial.println("HTTP server started");

  //******** Initialize sim808 module *************
  while (!sim808.init()) {
    delay(1000);
    Serial.print("Sim808 init error\r\n");
  }
  Serial.println("Sim808 init success");

  while (!sim808.attachGPS()) {
    Serial.println("wait for GPS to attach");
    delay(1000);
  } // wait for GPS to attach
  Serial.println("Open the GPS power success");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
