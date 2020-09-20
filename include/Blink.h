#ifndef BLINK_H
#define BLINK_H

#include <Arduino.h>
#include "AsyncPin.h"

class Led : public AsyncPin {

private:
    uint8_t _STATUS;

public:
    Led(uint8_t pin);

    void on();
    void off();
    void init();
    void blink(
        unsigned long _on_delay = 1000,
        unsigned long _off_delay = 1000,
        unsigned long _cycle_delay = 10);
};

#endif