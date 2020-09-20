#ifndef BLINK_H
#define BLINK_H

#include <Arduino.h>

class Led {
    private:
        uint8_t _pin;
        uint8_t _status = HIGH;
        unsigned long prev_ts;
        unsigned long curr_ts;

    public:
        Led(uint8_t pin);
  
        void on();
        void off();
        void setup();
        void blink(
                unsigned long _on_delay=1000,
                unsigned long _off_delay=1000,
                unsigned long _cycle_delay=10);
};

#endif