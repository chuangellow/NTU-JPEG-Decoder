#ifndef JPEGPARSER_H
#define JPEGPARSER_H

#include "BitReader.h"
#include "QuantizationTable.h"
#include "HuffmanTable.h"
#include <cstdint>
#include <vector>

class JPEGParser {
public:
    explicit JPEGParser(BitReader& reader);
    
    bool parseAPP0();
    bool parseSOF0();
    std::vector<HuffmanTable> parseDHT();
    std::vector<QuantizationTable> parseDQT();
    bool parseSOS();

private:
    BitReader& bitReader;
};

#endif
