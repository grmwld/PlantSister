#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#include "main.h"

#include "credentials.h"

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
const uint8_t DHTPin = D2;
// DHT Sensor
const uint8_t LED_INDICATOR = D0; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float temperature;
float humidity;

ESP8266WebServer server(8080);

void setup() {
  Serial.begin(9800);
  delay(100);

  pinMode(LED_INDICATOR, OUTPUT);

  pinMode(DHTPin, INPUT);
  dht.begin();
  connectToWifi();
  setupServer();
}

void loop() {
  server.handleClient();
  digitalWrite(LED_INDICATOR, HIGH);
  delay(10000);
  digitalWrite(LED_INDICATOR, LOW);
  delay(30);
}


void setupServer(void) {
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
void handle_OnConnect() {
  temperature = dht.readTemperature(); // Gets the values of the temperature
  humidity = dht.readHumidity(); // Gets the values of the humidity
  server.send(200, "text/html", SendHTML(temperature, humidity)); 
  Serial.print("Temperature: "); Serial.print((int)temperature); Serial.println(" *C");
  Serial.print("Humidity: "); Serial.print((int)humidity); Serial.println(" %");
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