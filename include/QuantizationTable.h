#ifndef QUANTIZATIONTABLE_H
#define QUANTIZATIONTABLE_H

#include <vector>
#include <cstdint>

class QuantizationTable {
public:
    QuantizationTable(uint8_t id, uint8_t precision);

    void setValue(int index, uint16_t value);
    uint8_t getTableID() const;
    uint16_t getValue(int index) const;

private:
    uint8_t tableID; // 0-3
    uint8_t precision; // 0-1
    std::vector<uint16_t> values;
};

#endif
