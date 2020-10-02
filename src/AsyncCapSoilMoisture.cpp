#include "AsyncCapSoilMoisture.h"

AsyncCapSoilMoisture::AsyncCapSoilMoisture(uint8_t pin) : AsyncPin(pin, INPUT) {
    init();
}

void AsyncCapSoilMoisture::init() {
    _airMoistureValue = 700.0;
    _waterMoistureValue = 350.0;
}

void AsyncCapSoilMoisture::getReadings() {
    _prev_ts = _curr_ts;
    _soilMoistureValue = analogRead(_PIN); // Gets moisture value
    if (_soilMoistureValue > _airMoistureValue) {
        _airMoistureValue = _soilMoistureValue;
    }
    if (_soilMoistureValue < _waterMoistureValue) {
        _waterMoistureValue = _soilMoistureValue;
    }
    _soilMoisturePercent = -100*(_soilMoistureValue-_airMoistureValue)/(_airMoistureValue-_waterMoistureValue);
}

void AsyncCapSoilMoisture::poll(unsigned long poll_delay, unsigned long cycle_delay, const std::function <void ()>& f) {
    delay(cycle_delay);
    _curr_ts = millis();
    if ((_curr_ts - _prev_ts >= poll_delay) || (_prev_ts == 0)) {
        getReadings();
        return f();
    }
}