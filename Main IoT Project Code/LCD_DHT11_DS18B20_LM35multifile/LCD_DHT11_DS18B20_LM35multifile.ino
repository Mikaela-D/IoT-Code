/* Mikaela Diaz
 * 13/03/23
 * Code to read humidity and temperature values from the DHT11 sensor. Also temperatures using a DS18B20 and an LM35. The values are printed to 
 * the serial monitor.
 * It also uses the library LiquidCrystal to print the readings to an LCD Display.
 * 
 * Code Sources: 
 * I got the DHT11 code from the web page with the link below, then I edited it to my preferences.
 * https://esp32io.com/tutorials/esp32-temperature-humidity-sensor
*/

#include "lcd.h"

float humi, tempC_DHT11, tempF_DHT11; //DHT11
float tempC_DS18B20, tempF_DS18B20; //DS18B20
float tempC_LM35, tempF_LM35; //LM35

// Create lcd object (instance)
LiquidCrystal lcd_w(RS, EN, D4, D5, D6, D7); //constructor is different than for arduino pins

// Create DHT object
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
 
void setup() {
  Serial.begin(115200); // ESP32 baud rate
  dht_sensor.begin(); // Initialise the DHT sensor
  sensors.begin(); // Start the DS18B20 sensor
  setupLcd();
}

void loop() {
   humi  = dht_sensor.readHumidity();
  // Read Celsius
   tempC_DHT11 = dht_sensor.readTemperature();
  // Read Fahrenheit
   tempF_DHT11 = dht_sensor.readTemperature(true);
  
  // Check if the reading is successful or not
  if ( isnan(tempC_DHT11) || isnan(tempF_DHT11) || isnan(humi)) { //If any of those statements are true print fail message
    Serial.println("Failed to read from DHT sensor!"); 
  } else {
    printValues_DHT11();   
  }
  delay(3000); // delay 2 seconds between readings
  lcd_w.clear();

  //DS18B20
  sensors.requestTemperatures(); 
  tempC_DS18B20 = sensors.getTempCByIndex(0);
  tempF_DS18B20 = sensors.getTempFByIndex(0);
  
  printValues_DS18B20();
  delay(3000); // delay 2 seconds between readings
  lcd_w.clear();

  //LM35
  int adcVal = analogRead(PIN_LM35); // Read ADC value from sensor
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION); // ADC value to millivolts
  
  // Voltage to °C
  tempC_LM35 = milliVolt / 10;
  // Convert °C to °F
  tempF_LM35 = (tempC_LM35 * 1.8) + 32;
  
  printValues_LM35();
  Serial.print("ADC value : "); // shows pre-scaled value
  Serial.print(adcVal); 
  Serial.print("\t\tMilli volts = "); // shows the voltage measured
  Serial.println(milliVolt);
  Serial.println(tempC_LM35);
  Serial.println(tempF_LM35);
  delay(3000); // delay 2 seconds between readings
  lcd_w.clear();
}
