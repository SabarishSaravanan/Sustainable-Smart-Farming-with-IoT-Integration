#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "sabari";
const char* password = "Sabarish02";
const char* weatherAPIKey = "API key"; //get API key from the openweather.org
const char* weatherLocation = "City name";
const int moistureSensorPin = A0; // Replace with the actual pin connected to the soil moisture sensor
const int motorPin = D2; // Replace with the actual pin connected to the motor

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

  pinMode(moistureSensorPin, INPUT);
  pinMode(motorPin, OUTPUT);
}

float getWeatherRainfall() {
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
        return 0.0;
      }

      float rainfall = doc["rain"]["1h"].as<float>();

      return rainfall;
    }
    else {
      Serial.printf("HTTP request failed with error code: %d\n", httpCode);
      return 0.0;
    }
  }

  http.end();

  return 0.0;
}

bool isSoilMoist() {
  int moistureLevel = analogRead(moistureSensorPin);
  
  // Adjust the threshold value based on your sensor and requirements
  if (moistureLevel > 500) {
    return false;
  } else {
    return true;
  }
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

void controlMotor(bool shouldTurnOnMotor) {
  digitalWrite(motorPin, shouldTurnOnMotor ? HIGH : LOW);
}

void loop() {
  float rainfall = getWeatherRainfall();
  bool isMoist = isSoilMoist();
  float temperature = getWeatherTemperature();

  Serial.print("Weather: ");
  Serial.println(rainfall);

  Serial.print("Moisture Sensor: ");
  Serial.println(isMoist);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Motor control logic
if (rainfall > 0.0 && !isMoist) {
  controlMotor(false); // Turn off the motor
  Serial.println("Motor turned off (Rain and No Moisture)");
} else if (rainfall == 0.0 && !isMoist) {
  controlMotor(true);  // Turn on the motor
  Serial.println("Motor turned on (No Rain and No Moisture)");
} else {
  controlMotor(false); // Turn off the motor
  Serial.println("Motor turned off (Other Conditions)");
}


  delay(5000);
}
