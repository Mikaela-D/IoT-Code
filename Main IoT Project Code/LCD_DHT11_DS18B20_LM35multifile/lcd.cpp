//lcd.cpp
//Mikaela Diaz

#include <DHT.h> 
#include "lcd.h"

extern float humi,tempC_DHT11, tempF_DHT11; //DHT11
extern float tempC_DS18B20, tempF_DS18B20; //DS18B20
extern float tempC_LM35, tempF_LM35; //LM35

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

//Special circle character for degree readings °C and °F
byte circle1[8] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setupLcd() {
  lcd_w.createChar(0, heart0); // create a new custom character (index 0)
  lcd_w.createChar(1, circle1); // create a new custom character (index 0)
  // set up the LCD's number of columns and rows:
  lcd_w.begin(16, 2); 
  
  // Print a message to the LCD.
  // set the cursor to column 0, line 0
  lcd_w.setCursor(0, 0);
  lcd_w.print("Health-Tracking");
  lcd_w.setCursor(0, 1);
  lcd_w.print("Smartwatch");
  lcd_w.setCursor(15, 1); // move cursor to (15, 1)
  lcd_w.write((byte)0);  // print the custom char at (15, 1)
  delay(3000);
  lcd_w.clear();
}

float printValues_DHT11() {
  lcd_w.setCursor(0, 0);
  lcd_w.print("Humidity: ");
  lcd_w.print(humi);
  lcd_w.print("%");

  lcd_w.setCursor(0, 1);
  lcd_w.print(tempC_DHT11);
  lcd_w.setCursor(5, 1); // move cursor to (5, 1)
  lcd_w.write((byte)1);  // print the custom char at (5, 1)
  lcd_w.setCursor(6, 1);
  lcd_w.print("C");

  lcd_w.setCursor(9, 1);
  lcd_w.print(tempF_DHT11);
  lcd_w.setCursor(14, 1);
  lcd_w.write((byte)1);
  lcd_w.print("F");
}

float printValues_DS18B20() {
  lcd_w.setCursor(0, 0);
  lcd_w.print("Temperature DS: ");

  lcd_w.setCursor(0, 1);
  lcd_w.print(tempC_DS18B20);
  lcd_w.setCursor(5, 1); // move cursor to (5, 1)
  lcd_w.write((byte)1);  // print the custom char at (5, 1)
  lcd_w.setCursor(6, 1);
  lcd_w.print("C");

  lcd_w.setCursor(9, 1);
  lcd_w.print(tempF_DS18B20);
  lcd_w.setCursor(14, 1);
  lcd_w.write((byte)1);
  lcd_w.print("F");
}

float printValues_LM35() {
  lcd_w.setCursor(0, 0);
  lcd_w.print("Temperature LM: ");

  lcd_w.setCursor(0, 1);
  lcd_w.print(tempC_LM35);
  Serial.println("Hello: ");
  Serial.println(tempC_LM35);
  lcd_w.setCursor(5, 1); // move cursor to (5, 1)
  lcd_w.write((byte)1);  // print the custom char at (5, 1)
  lcd_w.setCursor(6, 1);
  lcd_w.print("C");

  lcd_w.setCursor(9, 1);
  lcd_w.print(tempF_LM35);
  lcd_w.setCursor(14, 1);
  lcd_w.write((byte)1);
  lcd_w.print("F");
}
