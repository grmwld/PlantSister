#include <Arduino.h>
#include "AsyncPin.h"

AsyncPin::AsyncPin(uint8_t pin, uint8_t mode) {
    _PIN = pin;
    _MODE = mode;
    init();
}

void AsyncPin::init() {
    pinMode(_PIN, _MODE);
    _prev_ts = 0;
    _curr_ts = millis();
}