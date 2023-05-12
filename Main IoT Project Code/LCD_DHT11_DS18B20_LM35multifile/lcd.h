//lcd.h
//Mikaela Diaz

#ifndef LCD_H
#define LCD_H

// DHT11
#define DHT_SENSOR_PIN  4 // ESP32 pin GIOP4 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11

// LM35
#define ADC_VREF_mV    5000.0 // In millivolts
#define ADC_RESOLUTION 4096.0 // 12-bit
#define PIN_LM35       36 // ESP32 pin GIOP36 (ADC0) connected to LM35

#include <LiquidCrystal.h>
#include <DHT.h> 
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD Pin constants
const int RS = 13, EN = 12, D4 = 14, D5 = 27, D6 = 26, D7 = 25;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 0;  

extern LiquidCrystal lcd_w;

//Function Prototypes
void setupLcd();
float printValues_DHT11();
float printValues_DS18B20();
float printValues_LM35();

#endif
