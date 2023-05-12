//secrets.h
//Mikaela Diaz

#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>

const char* ssid       = ""; // Replace with your ssid
const char* password   = ""; // Replace with your password

const char* ntpServer = "europe.pool.ntp.org"; // NTP server pool
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600; // 3600 for DST (Daylight Saving time) vs 0 no saving time

// LCD Pin constants
const int RS = 13, EN = 12, D4 = 14, D5 = 27, D6 = 26, D7 = 25;

extern LiquidCrystal lcd;

#endif
