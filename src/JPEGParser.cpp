#include "JPEGParser.h"
#include "BitReader.h"

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

bool JPEGParser::parseDQT() {
    return true;
}

bool JPEGParser::parseSOS() {
    return true;
}
