#ifndef JPEGPARSER_H
#define JPEGPARSER_H

#include "BitReader.h"
#include "QuantizationTable.h"
#include "HuffmanTable.h"
#include "FrameParameter.h"
#include "ScanParameter.h"
#include <cstdint>
#include <vector>

class JPEGParser {
public:
    explicit JPEGParser(BitReader& reader);
    
    bool parseAPP0();
    FrameParameter parseSOF0();
    std::vector<HuffmanTable> parseDHT();
    std::vector<QuantizationTable> parseDQT();
    ScanParameter parseSOS();
    BitReader& bitReader;
};

#endif
