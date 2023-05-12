/* Mikaela Diaz
 * Date: 24/02/23
 * Description: This code prints date (words format) and time to the serial monitor and the LCD. 
 * This code has been adjusted to my preferences and geographical location.
 * Original code source: https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/ 
 */

#include <WiFi.h>
#include "time.h"
#include "secrets.h"

// Create lcd object (instance)
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //constructor is different than for arduino pins

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(&timeinfo, "%B %d %Y");
  lcd.setCursor(0, 1);
  lcd.print(&timeinfo, "%H:%M:%S");
}

void setup() {
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); 
  
  // Print a message to the LCD.
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("Health-Tracking");
  lcd.setCursor(0, 1);
  lcd.print("Smartwatch");
  lcd.setCursor(15, 1); // move cursor to (15, 1)
  lcd.write((byte)0);  // print the custom char at (15, 1)
  delay(3000);
  lcd.clear();
}

void loop() {
  delay(1000);
  printLocalTime();
}
