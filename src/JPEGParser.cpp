#include "JPEGParser.h"

JPEGParser::JPEGParser(BitReader& reader) : bitReader(reader) {}

bool JPEGParser::parseAPP0() {
    return true;
}

bool JPEGParser::parseSOF0() {
    return true;
}

bool JPEGParser::parseDHT() {
    return true;
}

std::vector<QuantizationTable> JPEGParser::parseDQT() {
    std::vector<QuantizationTable> tables;
    uint16_t length = bitReader.readWord() - 2;
    
    while (length > 0) {
        uint8_t qtInfo = bitReader.readByte();
        uint8_t precision = qtInfo >> 4;
        uint8_t tableID = qtInfo & 0x0F;

        QuantizationTable qt(tableID, precision);

        for (int i = 0; i < 64; ++i) {
            uint16_t qValue;
            if (precision == 0) {
                qValue = bitReader.readByte();
            }
            else {
                qValue = bitReader.readWord();
            }
            qt.setValue(i, qValue);
        }
        tables.push_back(qt);
        length -= (precision == 0) ? (1 + 64) : (1 + 64 * 2);
    }
    return tables;
}

bool JPEGParser::parseSOS() {
    return true;
}
