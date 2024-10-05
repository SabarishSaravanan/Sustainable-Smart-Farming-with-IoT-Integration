#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "MY";
const char* password = "menagayoga";
const char* weatherAPIKey = "5e3b2e4ec6c352007023cd9181c4171c";
const char* weatherLocation = "Salem";

const char* host = "api.openweathermap.org";
const int httpPort = 80;
const String url = "/data/2.5/weather?q=" + String(weatherLocation) + "&appid=" + String(weatherAPIKey);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());
}

float getWeatherHumidity() {
  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, host, httpPort, url);
  
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.println("Failed to parse JSON payload");
        return -1.0;
      }
      
      float humidity = doc["main"]["humidity"].as<float>();
      
      return humidity;
    }
    else {
      Serial.printf("HTTP request failed with error code: %d\n", httpCode);
      return -1.0;
    }
  }
  
  http.end();
  
  return -1.0;
}

float getWeatherTemperature() {
  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, host, httpPort, url);
  
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.println("Failed to parse JSON payload");
        return -100.0;
      }
      
      float temperature = doc["main"]["temp"].as<float>();
      temperature -= 273.15;
      
      return temperature;
    }
    else {
      Serial.printf("HTTP request failed with error code: %d\n", httpCode);
      return -100.0;
    }
  }
  
  http.end();
  
  return -100.0;
}

void loop() {
  float temperature = getWeatherTemperature();
  float humidity = getWeatherHumidity();
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  delay(5000);
}
