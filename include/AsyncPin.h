#ifndef ASYNCPIN_H
#define ASYNCPIN_H

#include <Arduino.h>

class AsyncPin {

protected:
    uint8_t _PIN;
    uint8_t _MODE;
    unsigned long _prev_ts;
    unsigned long _curr_ts;

public:
    AsyncPin(uint8_t pin, uint8_t mode);

    void init();
};

#endif