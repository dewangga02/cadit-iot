#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Replace with your network credentials
const char* ssid     = "French Revolution 2";
const char* password = "pearlharbor";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://dapur.manansfarmstore.com/post-data.php";
const int moisturePin = A0;
int moisturePercentage; 

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define SensorPin 2 
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header jumlah Q lahan
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    moisturePercentage = (analogRead(moisturePin)); // 1024.00) * 25);
    String data1 = String(moisturePercentage);
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(dht.readHumidity())
                           + "&value2=" + String(dht.readTemperature()) + "&value3=" + String(data1) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(120000);
  // Wait a few seconds between measurements.
  //delay(2000);


}
