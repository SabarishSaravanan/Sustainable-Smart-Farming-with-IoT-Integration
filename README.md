# Sustainable Smart Farming with IoT Integration

This project implements a smart irrigation and fertilizer control system using the NodeMCU ESP8266 microcontroller. It integrates real-time weather data from OpenWeather API, soil moisture sensor data, and automated controls to optimize water usage and nutrient delivery for efficient farming.

## Project Overview

This system automatically controls irrigation based on:
- **Soil moisture levels**
- **Weather forecast data** (rainfall predictions and temperature)

The project also integrates a fertilizer control mechanism that dispenses NPK nutrients when triggered. The goal is to conserve water by turning off the irrigation if rainfall is expected and ensure the soil receives adequate nutrients.

## Features
- **Weather-based Irrigation**: Fetches real-time weather data from OpenWeather API to check for expected rainfall and control the irrigation system accordingly.
- **Soil Moisture Sensing**: Monitors the soil moisture using an analog sensor and triggers irrigation if the soil is dry.
- **Fertilizer Control**: Dispenses NPK nutrients into the soil based on threshold inputs.
- **Real-time Monitoring**: Outputs the system status, including temperature, rainfall, and soil moisture conditions.

## Hardware Requirements
- **NodeMCU ESP8266**: Main controller for the system.
- **Soil Moisture Sensor**: To monitor the moisture content of the soil.
- **Relay Module**: To control the irrigation motor.
- **Water Pump**: For irrigation.
- **Fertilizer Motor**: For nutrient dispensing.
- **Jumper Wires**: For wiring connections.

## Software Requirements
- **Arduino IDE**: For uploading the code to NodeMCU.
- **ESP8266WiFi Library**: For connecting to WiFi.
- **ESP8266HTTPClient Library**: For making HTTP requests to fetch weather data.
- **ArduinoJson Library**: For parsing the JSON response from the OpenWeather API.
- **Adafruit MQTT Library**: For integrating MQTT for remote control.

## System Architecture

![System Architecture](https://github.com/SabarishSaravanan/Sustainable-Smart-Farming-with-IoT-Integration/blob/main/System%20Architecture.jpg)
## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contribution

Feel free to submit issues or pull requests for improvements. Contributions are welcome!
