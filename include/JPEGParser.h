#ifndef JPEGPARSER_H
#define JPEGPARSER_H

#include "BitReader.h"
#include <cstdint>

class JPEGParser {
public:
    explicit JPEGParser(BitReader& reader);
    
    bool parseAPP0();
    bool parseSOF0();
    bool parseDHT();
    bool parseDQT();
    bool parseSOS();

private:
    BitReader& bitReader;
};

#endif
