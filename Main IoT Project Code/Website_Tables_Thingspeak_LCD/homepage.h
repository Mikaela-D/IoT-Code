//homepage.h
//Mikaela Diaz

// Load Wi-Fi library
#include <WiFi.h>
#include <DHT.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

const char* ssid = ""; // Replace with your ssid
const char* password = ""; // Replace with your password  

// LCD Pin constants
const int RS = 13, EN = 12, D4 = 14, D5 = 27, D6 = 26, D7 = 25;

// DHT11 Channel
#define SECRET_CH_ID_DHT11 2018794      // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY_DHT11 "OS732T0827VBPXY5"   // replace XYZ with your channel write API Key

// DS18B20 Channel
#define SECRET_CH_ID_DS18B20 2062268      // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY_DS18B20 "0GK6QCAN9HYFCDLT"   // replace XYZ with your channel write API Key

//DHT11
#define DHT_SENSOR_PIN  4 // ESP32 pin GIOP4 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11
//LM35
#define ADC_VREF_mV    5000.0 // In millivolts
#define ADC_RESOLUTION 4096.0 // 12-bit
#define PIN_LM35       36 // ESP32 pin GIOP36 (ADC0) connected to LM35

// Display the HTML web page using CSS
// Using flash memory to store non-changing parts of the web page
String mainPage = (F(R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale = 1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Amasis MT Pro Medium;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
            color: black;
            background-color: rgb(188, 248, 203);
        }

        p {
            font-size: 180%;
            font-family: Arial, Helvetica, sans-serif;
        }

        .button {
            background-color: #ec0909;
            border: 5px solid black;
            padding: 3px;
            color: white;
            padding: 22px 60px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
        }

        table {
            font-family: arial, sans-serif;
            border-collapse: collapse;
            width: 70%;
        }

        table,
        td,
        th {
            border: 2px solid #F20019;
            text-align: center;
            padding: 15px;
        }

        table th {
            background-color: #fad3a0;
        }

        tr:nth-child(even) {
            background-color: #faf287;
        }

        table.center {
            margin-left: auto;
            margin-right: auto;
        }

        @media (max-width: 800px) {
            .flex-container {
                flex-direction: column;
            }
        }
    </style>
    <title>Health-Tracking Webpage</title>
    <meta http-equiv="refresh" content="20">
</head>
<body>
    <div id='main'>
        <h1 style="font-size: 250%;">Health-Tracking Website with ESP32 Server</h1>
        <p><b><i>Welcome to your Homepage</i></b></p>
        <img src="https://live.staticflickr.com/65535/52696081361_533bc99af1_z.jpg" width="600" height="340"
             alt="Health Tracking Smartwatch LCD Screen Image">
        <script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8">
        </script>
        <p style="font-size: 120%;">Here are some readings of your Health-Tracking Smartwatch.</p>
    </div>
        <h2>Send Location Button</h2>
    <div class="flex-keypad">
        <button class="button">SMS</button>
    </div>
</body>
)====="));
