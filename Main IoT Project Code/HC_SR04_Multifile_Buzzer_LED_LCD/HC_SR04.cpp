//HC_SR04.cpp
//Mikaela Diaz

#include "HC_SR04.h"

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

byte exclamation1[8] = {
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110
};

byte exclamation2[8] = {
  B01110,
  B01110,
  B01110,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110
};

void configHCSR_04Pins()
{
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  // create a new custom character (index 0)
  lcd.createChar(0, heart0); 
  lcd.createChar(1, exclamation1); 
  lcd.createChar(2, exclamation2); 
  
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
float measureDistanceCms()
{
  long _duration;
  float _cms;

  // The sensor is triggered by a HI pulse of 10 or greater microsec.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pin: a HI pulse whose duration is the time (in microsec) from the sending
  // of the trig to the reception of its echo off of an object.
  _duration = pulseIn(ECHO_PIN, HIGH);

  // Convert the time into a distance
  _cms = (_duration / 2) * SPEED_OF_SOUND_AT_20C_CM;   // Divide by 29.1 or multiply by 0.0343
  return _cms;
}
float measureDistanceInches()
{
  long _duration;
  float _inches;

  // The sensor is triggered by a HI pulse of 10 or greater microsec.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pin: a HI pulse whose duration is the time (in microsec) from the sending
  // of the trig to the reception of its echo off of an object.
  _duration = pulseIn(ECHO_PIN, HIGH);

  // Convert the time into a distance
  _inches = (_duration / 2) * SPEED_OF_SOUND_AT_20C_IN; // Divide by 74 or multiply by 0.0135
  return _inches;
}
void printInches(float _inches)
{
  Serial.print("Distance from object in inches ");
  Serial.println( _inches);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("In : ");
  lcd.setCursor(5, 1);
  lcd.print( _inches);
}
void printCms(float _cms)
{
  Serial.print("Distance from object in cms ");
  Serial.println( _cms);
  lcd.setCursor(0, 0);
  lcd.print("Cm : ");
  lcd.setCursor(5, 0);
  lcd.print( _cms);
}
void distanceWarning(float _cms)
{
  if( _cms <= 60) 
  {
    Serial.println("Warning, too close");
    digitalWrite (BUZZER, HIGH); //turn buzzer on
    digitalWrite (LED, HIGH); //turn buzzer on
    lcd.setCursor(15, 0); // move cursor to (15, 1)
    lcd.write((byte)1);  // print the custom char at (15, 1)
    lcd.setCursor(15, 1); // move cursor to (15, 1)
    lcd.write((byte)2);  // print the custom char at (15, 1)
  }
  else
  {
    digitalWrite (BUZZER, LOW);  //turn buzzer off
    digitalWrite (LED, LOW);  //turn buzzer off
  }
}
