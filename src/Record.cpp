#include "Record.h"

Record::Record(float value, unsigned long timestamp)
    : _value(value), _timestamp(timestamp) {}

Record::JsonDocument Record::serialize() {
    Record::JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    root["value"] = _value;
    root["timestamp"] = _timestamp;
    return doc;
}

String Record::serializeToString() {
    String result;
    auto doc = this->serialize();
    serializeJson(doc, result);
    return result;
}