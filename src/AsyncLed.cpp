#include <Arduino.h>
#include "AsyncLed.h"

Led::Led(uint8_t pin) : AsyncPin(pin, OUTPUT) {
    init();
}

void Led::init() {
    _STATUS = HIGH;
}

void Led::on() {
    _STATUS = LOW;
    digitalWrite(_PIN, LOW);
}

void Led::off() {
    _STATUS = HIGH;
    digitalWrite(_PIN, HIGH);
}

void Led::blink(unsigned long on_delay, unsigned long off_delay, unsigned long cycle_delay) {
    delay(cycle_delay);
    _curr_ts = millis();
    if (_STATUS == LOW) { // The led is on
        if (_curr_ts - _prev_ts >= on_delay) {
            _prev_ts = _curr_ts;
            off();
        }
    }
    else { // The led is off
        if (_curr_ts - _prev_ts >= off_delay) {
            _prev_ts = _curr_ts;
            on();
        }
    }
 }