#ifndef ASYNCCAPSOILMOISTURE_H
#define ASYNCCAPSOILMOISTURE_H

#include <Arduino.h>
#include "AsyncPin.h"

class AsyncCapSoilMoisture : public AsyncPin {

private:
    float _airMoistureValue;
    float _waterMoistureValue;
    float _soilMoistureValue;
    float _soilMoisturePercent;

public:
    AsyncCapSoilMoisture(uint8_t pin);

    void init();
    void getReadings();
    void poll(
        unsigned long poll_delay,
        unsigned long cycle_delay,
        const std::function <void ()>& f);

    float getSoilMoisturePercent() {return _soilMoisturePercent;}
    float getSoilMoistureValue() {return _soilMoistureValue;}
};

#endif