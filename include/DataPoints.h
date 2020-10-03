#ifndef DATAPOINTS_H
#define DATAPOINTS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <CircularBuffer.h>
#include "Record.h"

#define MAX_RECORDS 96

class DataPoints : public CircularBuffer<Record*, MAX_RECORDS, uint8_t> {

    using CircularBuffer<Record*, MAX_RECORDS, uint8_t>::CircularBuffer;
    using JsonDocument = StaticJsonDocument<1*JSON_ARRAY_SIZE(MAX_RECORDS) + 2*JSON_OBJECT_SIZE(2)>;

public:

    JsonDocument serialize();
    String serializeToString();
    
    Record* get(uint8_t index) const;
};

#endif
