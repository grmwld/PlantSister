#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "main.h"
#include "AsyncDHT.h"
#include "AsyncLed.h"

#include "credentials.h"

// DHT Sensor
const uint8_t DHT_PIN = D2;
const uint8_t DHT_TYPE = DHT22;
AsyncDHT dht(DHT_PIN, DHT_TYPE);

// LED Indicator
const uint8_t LED_INDICATOR = D0;
Led led_indicator(LED_INDICATOR);
               
// Initialize DHT sensor.
// DHT dht(DHTPin, DHTTYPE);

float temperature;
float humidity;

AsyncWebServer server(8080);

void setup() {
  Serial.begin(9800);
  delay(100);
  
  connectToWifi();
  setupServer();
}

void loop() {
  led_indicator.blink(5, 4000);
  dht.poll();
}


void setupServer(void) {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", SendHTML(dht.getTemperature(), dht.getHumidity())); 
    Serial.print("Temperature: "); Serial.print((int)dht.getTemperature()); Serial.println(" *C");
    Serial.print("Humidity: "); Serial.print((int)dht.getHumidity()); Serial.println(" %");
  });
  server.begin();
  Serial.println("HTTP server started");
}

String SendHTML(float temperature, float humidity) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Plant Sister</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Plant Sister</h1>\n";
  
  ptr +="<p>Temp&eacute;rature : ";
  ptr +=(int)temperature;
  ptr +=" &#176;C</p>";
  ptr +="<p>Humidit&eacute; : ";
  ptr +=(int)humidity;
  ptr +=" %</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
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