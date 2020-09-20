#include <Arduino.h>
#include "Blink.h"

Led::Led(uint8_t pin) {
    _pin = pin;
}

void Led::on() {
    _status = LOW;
    digitalWrite(_pin, LOW);
}

void Led::off() {
    _status = HIGH;
    digitalWrite(_pin, HIGH);
}

void Led::setup() {
    pinMode(_pin, OUTPUT);
    prev_ts = 0;
    curr_ts = millis();
}

void Led::blink(unsigned long _on_delay, unsigned long _off_delay, unsigned long _cycle_delay) {
    delay(_cycle_delay);
    curr_ts = millis();
    if (_status == LOW) { // The led is on
        if (curr_ts - prev_ts >= _on_delay) {
            prev_ts = curr_ts;
            off();
        }
    }
    else { // The led is off
        if (curr_ts - prev_ts >= _off_delay) {
            prev_ts = curr_ts;
            on();
        }
    }
 }