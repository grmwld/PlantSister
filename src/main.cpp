#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
// #include <CircularBuffer.h>

#include "main.h"
#include "AsyncDHT.h"
#include "AsyncLed.h"
#include "AsyncCapSoilMoisture.h"
#include "DataPoints.h"

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
const long utcOffsetInSeconds = 2 * 3600;
const long updateInterval = 24 * 3600;
unsigned long lastUpdateEpoch;
String lastUpdateFormatedTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, updateInterval);

// Semi persistent data
// #define MAX_RECORDS 96
DataPoints temperatureRecords;
DataPoints humidityRecords;
DataPoints moisturepcRecords;
DataPoints moisturevlRecords;

AsyncWebServer server(8080);

void setup() {
  Serial.begin(9600);
  delay(100);
  
  connectToWifi();

  timeClient.begin();
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

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
  dht.poll(1*55*1000, 10, []() {
    timeClient.update();
    lastUpdateFormatedTime = timeClient.getFormattedTime();
    lastUpdateEpoch = timeClient.getEpochTime();
    temperatureRecords.push(new Record(dht.getTemperature(), lastUpdateEpoch));
    humidityRecords.push(new Record(dht.getHumidity(), lastUpdateEpoch));
  });
  csm.poll(1*55*1000, 10, []() {
    timeClient.update();
    lastUpdateFormatedTime = timeClient.getFormattedTime();
    lastUpdateEpoch = timeClient.getEpochTime();
    moisturepcRecords.push(new Record(csm.getSoilMoisturePercent(), lastUpdateEpoch));
    moisturevlRecords.push(new Record(csm.getSoilMoistureValue(), lastUpdateEpoch));
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
  server.on("/temperature/last", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", temperatureRecords.last()->serializeToString().c_str());
  });
  server.on("/temperature/history", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", temperatureRecords.serializeToString().c_str());
  });
  server.on("/humidity/last", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", humidityRecords.last()->serializeToString().c_str());
  });
  server.on("/humidity/history", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", humidityRecords.serializeToString().c_str());
  });
  server.on("/moisturepc/last", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", moisturepcRecords.last()->serializeToString().c_str());
  });
  server.on("/moisturepc/history", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", moisturepcRecords.serializeToString().c_str());
  });
  server.on("/moisturevl/last", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", moisturevlRecords.last()->serializeToString().c_str());
  });
  server.on("/moisturevl/history", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "application/json", moisturevlRecords.serializeToString().c_str());
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
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404);
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