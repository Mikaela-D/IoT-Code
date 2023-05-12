//HC_SR04.h                                                                                                                                                                                                                               
//Mikaela Diaz

// Constants and Function Prototypes for HCSR04

#ifndef HC_SR04_h
#define HC_SR04_h

#include <Arduino.h>
#include <LiquidCrystal.h>

#define SPEED_OF_SOUND_AT_20C_CM 0.0343
#define SPEED_OF_SOUND_AT_20C_IN 0.0135

// LCD Pin constants
const int RS = 13, EN = 12, D4 = 14, D5 = 27, D6 = 26, D7 = 25;

const int TRIG_PIN = 5;    // Trigger
const int ECHO_PIN = 18;    // Echo
const int BUZZER = 15; //for ESP32 Microcontroller
const int LED = 2;

extern LiquidCrystal lcd;

// Function Prototypes
void configHCSR_04Pins();
float measureDistanceCms();
float measureDistanceInches();
void printInches(float _inches);
void printCms(float _cms);
void distanceWarning(float _cms);

#endif
