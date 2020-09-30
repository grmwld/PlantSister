#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "main.h"
#include "AsyncDHT.h"
#include "AsyncLed.h"
#include "AsyncCapSoilMoisture.h"

#include "credentials.h"

// DHT Sensor
const uint8_t DHT_PIN = D2;
const uint8_t DHT_TYPE = DHT22;
AsyncDHT dht(DHT_PIN, DHT_TYPE);

// Capacitive soil moisture sensor
const uint8_t CSM_PIN = A0;
AsyncCapSoilMoisture csm(CSM_PIN);

// LED Indicator
const uint8_t LED_INDICATOR = D0;
Led led_indicator(LED_INDICATOR);

// Date & time related settings
// const long utcOffsetInSeconds = 2 * 3600;
// const long updateInterval = 24 * 3600;
// String lastUpdateFormatedTime;
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, updateInterval);

AsyncWebServer server(8080);

void setup() {
  Serial.begin(9600);
  delay(100);
  
  connectToWifi();
  // timeClient.begin();
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  setupOTA();

  setupServer();
}

void loop() {
  ArduinoOTA.handle();
  led_indicator.blink(10, 4000);
  dht.poll(1*60*1000, 10, []() {
    // timeClient.update();
    // lastUpdateFormatedTime = timeClient.getFormattedTime();
  });
  csm.poll(1*60*1000, 10, []() {
    // timeClient.update();
    // lastUpdateFormatedTime = timeClient.getFormattedTime();
    // Serial.print("Soil Moisture: "); Serial.println((int)csm.getSoilMoistureValue());
    // Serial.print("Soil Moisture: "); Serial.print((int)csm.getSoilMoisturePercent()); Serial.println(" %");
  });
}


void setupOTA() {
  ArduinoOTA.setHostname("PlantSister");
  ArduinoOTA.setPassword("iswatchingyou");

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void setupServer(void) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
    Serial.print("Temperature: "); Serial.print((int)dht.getTemperature()); Serial.println(" *C");
    Serial.print("Humidity: "); Serial.print((int)dht.getHumidity()); Serial.println(" %");
    Serial.print("Soil Moisture: "); Serial.print((int)csm.getSoilMoisturePercent()); Serial.println(" %");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", ((String)dht.getTemperature()).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", ((String)dht.getHumidity()).c_str());
  });
  server.on("/moisturepc", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", ((String)csm.getSoilMoisturePercent()).c_str());
  });
  server.on("/moisturevl", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", ((String)csm.getSoilMoistureValue()).c_str());
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/highcharts.js", "text/javascript");
  });
  server.on("/plot-data.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/plot-data.js", "text/javascript");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/manifest.webmanifest", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/manifest.webmanifest", "application/json");
  });
  server.begin();
  Serial.println("HTTP server started");
}

void connectToWifi() {
  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi connection status
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected !");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}