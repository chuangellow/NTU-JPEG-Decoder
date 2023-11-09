#include "QuantizationTable.h"

QuantizationTable::QuantizationTable(uint8_t id, uint8_t prec) 
    : tableID(id), precision(prec), values(64, 0) {}

void QuantizationTable::setValue(int index, uint16_t value) {
    if (index >= 0 && index < values.size()) {
        values[index] = value;
    }
}

uint8_t QuantizationTable::getTableID() const {
    return tableID;
}

uint8_t QuantizationTable::getPrecision() const {
    return precision;
}

uint16_t QuantizationTable::getValue(int index) const {
    if (index >= 0 && index < values.size()) {
        return values[index];
    }
    return 0;
}
