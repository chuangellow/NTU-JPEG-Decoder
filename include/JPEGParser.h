#ifndef JPEGPARSER_H
#define JPEGPARSER_H

#include "BitReader.h"
#include "QuantizationTable.h"
#include <cstdint>
#include <vector>

class JPEGParser {
public:
    explicit JPEGParser(BitReader& reader);
    
    bool parseAPP0();
    bool parseSOF0();
    bool parseDHT();
    std::vector<QuantizationTable> parseDQT();
    bool parseSOS();

private:
    BitReader& bitReader;
};

#endif
