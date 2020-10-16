#ifndef RECORD_H
#define RECORD_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Record {

    using JsonDocument = StaticJsonDocument<JSON_OBJECT_SIZE(2)>;

private:
    const unsigned long _timestamp;
    const float _value;

public:
    Record(float value, unsigned long timestamp);
    ~Record() {};

    JsonDocument serialize();
    String serializeToString();

    unsigned long timestamp() {return this->_timestamp;}
    float value() {return this->_value;}
};

#endif