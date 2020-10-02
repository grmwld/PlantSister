#ifndef DATAPOINTS_H
#define DATAPOINTS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <CircularBuffer.h>

template<typename T, size_t S, typename IT = typename Helper::Index<(S <= UINT8_MAX), (S <= UINT16_MAX)>::Type>
class DataPoints : public CircularBuffer<T, S, IT> {

    using CircularBuffer<T, S, IT>::CircularBuffer;

public:

//     JsonDocument serialize() {
//         const int CAPACITY = 1*JSON_ARRAY_SIZE(this->capacity) + 2*JSON_OBJECT_SIZE(2);
//         StaticJsonDocument<CAPACITY> doc;
//         for (int i = 0; i < this->size(); i++) {
//             doc.add(this[i].serialize());
//         }
//         return doc;
//     }

//     String serializeToString() {
//         String result;
//         serializeJson(this->serialize(), result);
//         return result;
//     }
    void print() {
        Serial.print("Size of dataset: "); Serial.println(this->size());
        // for (int i = 0; i < this->size(); i++) {
        //     Serial.print("item from inloop: "); Serial.println(this[i].timestamp());
        //     this[i].print();
        // }
    }
};

// #include "DataPoints.h"
// template<typename T, size_t S, typename IT>
// String DataPoints<T, S, IT>::serialize() {}

#endif
