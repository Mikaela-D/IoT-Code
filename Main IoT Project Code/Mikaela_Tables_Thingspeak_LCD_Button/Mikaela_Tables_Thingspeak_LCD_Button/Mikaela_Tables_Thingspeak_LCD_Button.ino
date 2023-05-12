/*
  Mikaela Diaz
  17/03/23
  Description: Web page code from an ESP32 server to display DHT11, DS18B20, LM35 readings which can be refreshed by a New Reading button.
  The web page is able to display Thingspeak graphs.
  Code Source: Example given by lecturer. The code has been edited to fit my web page with my DHT11 sensor.
  Complete project details at http://randomnerdtutorials.com
*/

#include "code.h"
#include "ThingSpeak.h"

// Create an instance of DHT sensor (constructor).
// This line will create the object of DHT according to our defined pin number and DHT type.
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

float humi;

int keyIndex = 0;
WiFiClient  client;
String myStatus = "";

// DHT11
unsigned long myChannelNumber_DHT11 = SECRET_CH_ID_DHT11;
const char * myWriteAPIKey_DHT11 = SECRET_WRITE_APIKEY_DHT11;
// DS18B20
unsigned long myChannelNumber_DS18B20 = SECRET_CH_ID_DS18B20;
const char * myWriteAPIKey_DS18B20 = SECRET_WRITE_APIKEY_DS18B20;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 0;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Create lcd object (instance)
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //constructor is different than for arduino pins

// Set web server port number to 80
WiFiServer server(80);

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

// Variable to store the HTTP request
String header;

void setup() {
  Serial.begin(115200);
  dht_sensor.begin(); // Initialise the DHT sensor
  sensors.begin(); // Start the DS18B20 sensor

  lcd.createChar(0, heart0); // create a new custom character (index 0)
  lcd.createChar(1, circle1); // create a new custom character (index 0)
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("Health-Tracking");
  lcd.setCursor(0, 1);
  lcd.print("Smartwatch");
  lcd.setCursor(15, 1); // move cursor to (15, 1)
  lcd.write((byte)0);  // print the custom char at (15, 1)

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  server.begin(); // Initialise server
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  lcd.clear();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.print("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() ) {           // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Natasha.. check if button is pressed and send SMS if it is. Then serve webpage.
            if (header.indexOf("GET /on") >= 0) {
              Serial.println("Sending SMS message");

            }
            client.println(mainPage);

            // DHT11 Code
            humi  = dht_sensor.readHumidity(); // Read humidity
            float tempC_DHT11 = dht_sensor.readTemperature(); // Read Celsius
            float tempF_DHT11 = dht_sensor.readTemperature(true); // Read Fahrenheit

            // Check if the reading is successful or not
            if ( isnan(tempC_DHT11) || isnan(tempF_DHT11) || isnan(humi)) { //If any of those statements are true print fail message
              Serial.println("Failed to read from DHT sensor!");
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Humidity: ");
              lcd.print(humi);
              lcd.print("%");
              lcd.setCursor(0, 1);
              lcd.print(tempC_DHT11);
              lcd.setCursor(5, 1); // move cursor to (5, 1)
              lcd.write((byte)1);  // print the custom char at (5, 1)
              lcd.setCursor(6, 1);
              lcd.print("C");
              lcd.setCursor(9, 1);
              lcd.print(tempF_DHT11);
              lcd.setCursor(14, 1);
              lcd.write((byte)1);
              lcd.print("F");
            }
            String tableDHT11 = (F(R"=====(
            <div class="flex-container">
            <p style = "font-size: 200%;">Humidity and Temperature sensor Table DHT11</p>
            <table class="center">
              <tr>   
                <th>Humidity</th>
                <th>Degrees Celcius</th>
                <th>Degrees Fahrenheit</th>
              </tr>        
            )====="));
            client.println(tableDHT11);

            //Display DHT11 Readings
            client.print("<td>");
            client.print(humi);
            client.print("%");
            client.print("</td>");
            client.print("<td>");
            client.print(tempC_DHT11);
            client.print("</td>");
            client.print("<td>");
            client.print(tempF_DHT11);
            client.print("</td>");
            client.print("</table>");

            // DS18B20 code
            sensors.requestTemperatures();
            float tempDS_C = sensors.getTempCByIndex(0);
            float tempDS_F = sensors.getTempFByIndex(0);

            if ( isnan(tempDS_C) || isnan(tempDS_F)) { //If any of those statements are true print fail message
              Serial.println("Failed to read from DS18B20 sensor!");
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Temperature DS: ");
              lcd.setCursor(0, 1);
              lcd.print(tempDS_C);
              lcd.setCursor(5, 1); // move cursor to (5, 1)
              lcd.write((byte)1);  // print the custom char at (5, 1)
              lcd.setCursor(6, 1);
              lcd.print("C");
              lcd.setCursor(9, 1);
              lcd.print(tempDS_F);
              lcd.setCursor(14, 1);
              lcd.write((byte)1);
              lcd.print("F");
            }

            String tableDS18B20 = (F(R"=====(
            <p style = "font-size: 200%;">Body Temperature sensor Table DS18B20</p>
            <table class="center">
              <tr>   
                <th>Degrees Celcius</th>
                <th>Degrees Fahrenheit</th>
              </tr>        
            )====="));
            client.print(tableDS18B20);

            // Display DS18B20 readings
            client.print("<td>");
            client.print(tempDS_C);
            client.print("</td>");
            client.print("<td>");
            client.print(tempDS_F);
            client.print("</td>");
            client.print("</table>");

            // LM35 code
            int adcVal = analogRead(PIN_LM35); // Read ADC value from sensor
            float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION); // ADC value to millivolts

            // Voltage to °C
            float tempLM_C = milliVolt / 10;
            // Convert °C to °F
            float tempLM_F = (tempLM_C * 1.8) + 32;

            if ( isnan(tempLM_C) || isnan(tempLM_F)) { //If any of those statements are true print fail message
              Serial.println("Failed to read from DS18B20 sensor!");
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Temperature LM: ");
              lcd.setCursor(0, 1);
              lcd.print(tempLM_C);
              lcd.setCursor(5, 1); // move cursor to (5, 1)
              lcd.write((byte)1);  // print the custom char at (5, 1)
              lcd.setCursor(6, 1);
              lcd.print("C");
              lcd.setCursor(9, 1);
              lcd.print(tempLM_F);
              lcd.setCursor(14, 1);
              lcd.write((byte)1);
              lcd.print("F");
            }

            String tableLM35 = (F(R"=====(
            <p style = "font-size: 200%;">Body Temperature sensor Table LM35</p>
            <table class="center">
              <tr>   
                <th>Degrees Celsius</th>
                <th>Degrees Fahrenheit</th>
              </tr>        
            )====="));
            client.print(tableLM35);

            // Display LM35 readings
            client.print("<td>");
            client.print(tempLM_C);
            client.print("</td>");
            client.print("<td>");
            client.print(tempLM_F);
            client.print("</td>");
            client.print("</table>");
            client.print("</div>");

            // set the fields with the values for DHT11
            ThingSpeak.setField(1, humi);
            ThingSpeak.setField(2, tempC_DHT11);
            ThingSpeak.setField(3, tempF_DHT11);

            // figure out the status message
            if (tempC_DHT11 > 30 && humi > 90) {
              myStatus = String("Temperature and Humidity are too high");
            }
            else if (tempC_DHT11 < 5 && humi < 35) {
              myStatus = String("Temperature and Humidity are too low");
            }
            else {
              myStatus = String("Temperature and Humidity are fine");
            }
            // set the status
            ThingSpeak.setStatus(myStatus);

            // write to the DHT11 ThingSpeak channel (all data together)
            int x = ThingSpeak.writeFields(myChannelNumber_DHT11, myWriteAPIKey_DHT11);
            if (x == 200) {
              Serial.println("Channel update successful for DHT11.");

              String DHT11_Graphs = (F(R"=====(
              <div>
                <p>Humidity and Temperature Graphs DHT11</p>
                <br>
                <iframe width="450" height="260" style="border: 2px solid black;"
                    src="https://thingspeak.com/channels/2018794/charts/1?bgcolor=%23FCFEEE&color=%238109FF&dynamic=true&results=60&title=Humidity&type=line">
                </iframe>
                </br>
                <iframe width="450" height="260" style="border: 2px solid black;"
                    src="https://thingspeak.com/channels/2018794/charts/2?bgcolor=%23FCFEEE&color=%238109FF&dynamic=true&results=60&title=Temperature+in+Celsius&type=line">
                </iframe>
                <iframe width="450" height="260" style="border: 2px solid black;"
                    src="https://thingspeak.com/channels/2018794/charts/3?bgcolor=%23FCFEEE&color=%238109FF&dynamic=true&results=60&title=Temperature+in+Fahrenheit&type=line">
                </iframe>                
              </div>
              )====="));
              client.println(DHT11_Graphs);
            } else {
              Serial.println("Problem updating channel for DHT11. HTTP error code " + String(x));
            }

            // set the fields with the values
            ThingSpeak.setField(1, tempDS_C);
            ThingSpeak.setField(2, tempDS_F);

            // write to the DS18B20 ThingSpeak channel (all data together)
            if (x == 200) {
              Serial.println("Channel update successful for DS18B20.");

              String DS18B20_Graphs = (F(R"=====(
              <div>
                <p>Temperature Graphs DS18B20</p>
                <iframe width="450" height="260" style="border: 2px solid black;"
                    src="https://thingspeak.com/channels/2062268/charts/1?bgcolor=%23E7E9FF&color=%23018000&dynamic=true&results=60&title=Temperature+in+Celsius&type=line">
                </iframe>
                <iframe width="450" height="260" style="border: 2px solid black;"
                    src="https://thingspeak.com/channels/2062268/charts/2?bgcolor=%23E7E9FF&color=%23018000&dynamic=true&results=60&title=Temperature+in+Fahrenheit&type=line">
                </iframe>
              </div>
              )====="));
              client.println(DS18B20_Graphs);
              client.print("</html>");
            } else {
              Serial.println("Problem updating channel for DS18B20. HTTP error code " + String(x));
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // change the values
    humi++;
    if (humi > 99) {
      humi = 0;
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
