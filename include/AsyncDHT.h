#ifndef ASYNCDHT_H
#define ASYNCDHT_H

#include <Arduino.h>
#include "AsyncPin.h"
#include "DHT.h"

class AsyncDHT : public AsyncPin {

private:
    float _temperature;
    float _humidity;
    DHT _dht;

public:
    AsyncDHT(uint8_t pin, uint8_t dhttype);

    void init();
    void getReadings();
    void poll(
        unsigned long poll_delay=5000,
        unsigned long cycle_delay=50);

    float getTemperature() {return _temperature;}
    float getHumidity() {return _humidity;}
};

#endif