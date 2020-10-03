#include "DataPoints.h"

DataPoints::JsonDocument DataPoints::serialize() {
    DataPoints::JsonDocument doc;
    for (int i = 0; i < this->size(); i++)
    {
        auto objdoc = this->get(i)->serialize();
        doc.add(objdoc);
    }
    return doc;
}

String DataPoints::serializeToString() {
    String result;
    auto doc = this->serialize();
    serializeJson(doc, result);
    return result;
}

Record* DataPoints::get(uint8_t index) const {
	if (index >= count) return *tail;
	return *(buffer + ((head - buffer + index) % capacity));
}