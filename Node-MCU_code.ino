#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_MQTT_Client.h>

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* weatherAPIKey = "API KEY"; //get API key from the openweather.org
const char* weatherLocation = "CITY NAME"; 
const int moistureSensorPin = A0; // Replace with the actual pin connected to the soil moisture sensor
const int motorPin = D2; // Replace with the actual pin connected to the motor

const int FertilizerMotor = D3; // Pin connected to the motor
int threshold = 80;      // Threshold value
unsigned long motorStartTime = 0;
const unsigned long motorDuration = 5000; // 5 seconds


const char* host = "api.openweathermap.org";
const int httpPort = 80;
const String url = "/data/2.5/weather?q=" + String(weatherLocation) + "&appid=" + String(weatherAPIKey);

void setup() {
  Serial.begin(115200);
  pinMode(FertilizerMotor, OUTPUT);
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

void loop() 
{

    if (Serial.available() > 0) 
 {
    int value = Serial.parseInt(); // Read the integer value from Serial Monitor

    if (value < threshold) 
    {
      digitalWrite(FertilizerMotor, HIGH); // Turn on the motor
      motorStartTime = millis();    // Record the start time
      Serial.println("FertilizerMotor turned on!");

      // Wait for 5 seconds
      delay(motorDuration);

      // Turn off the motor after 5 seconds
      digitalWrite(FertilizerMotor, LOW); // Turn off the motor
      Serial.println("FertilizerMotor turned off.");
    }
 } 


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

  
  // Scenario 1: Turn off the motor if it's raining and the soil is dry
  if (rainfall > 0.0 && !isMoist) {
    controlMotor(false); // Turn off the motor
    Serial.println("Scenario 1: Motor turned off,it's raining and the soil is dry");
  }
  // Scenario 2: Turn off the motor if it's raining and the soil is moist
  else if (rainfall > 0.0 && isMoist) {
    controlMotor(false); // Turn off the motor
    Serial.println("Scenario 2: Motor turned off,it's raining and the soil is moist");
  }
  // Scenario 3: Turn on the motor if it's not raining and the soil is dry
  else if (rainfall == 0.0 && !isMoist) {
    controlMotor(true); // Turn on the motor
    Serial.println("Scenario 3: Motor turned on,it's not raining and the soil is dry");
  }
  // Scenario 4: Turn off the motor if it's not raining and the soil is moist
  else if (rainfall == 0.0 && isMoist) {
    controlMotor(false); // Turn off the motor
    Serial.println("Scenario 4: Motor turned off,it's not raining and the soil is moist");
  }

  delay(5000);
}
