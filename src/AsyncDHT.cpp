#include <Arduino.h>
#include "AsyncDHT.h"

AsyncDHT::AsyncDHT(uint8_t pin, uint8_t dhttype) : AsyncPin(pin, INPUT), _dht(pin, dhttype) {
    init();
}

void AsyncDHT::init() {
    _temperature = -196.0;
    _humidity = -1.0;
    _dht.begin();
}

void AsyncDHT::getReadings() {
    _prev_ts = _curr_ts;
    float t = _dht.readTemperature();
    float h = _dht.readHumidity();
    _temperature = isnan(t) ? _temperature : t; // Gets temperature value
    _humidity = isnan(h) ? _temperature : h; // Gets humidity value
}

void AsyncDHT::poll(unsigned long poll_delay, unsigned long cycle_delay, const std::function <void ()>& f) {
    delay(cycle_delay);
    _curr_ts = millis();
    if ((_curr_ts - _prev_ts >= poll_delay) || (_prev_ts == 0)) {
        getReadings();
        return f();
    }
}