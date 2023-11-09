#ifndef JPEGDECODER_H
#define JPEGDECODER_H

#include <string>
#include <iostream>
#include "BitReader.h"
#include "JPEGParser.h"
#include "QuantizationTable.h"
#include "HuffmanTable.h"

const uint8_t JPEG_PREFIX = 0xFF;
const uint8_t JPEG_APP0 = 0xE0;
const uint8_t JPEG_SOI = 0xD8;
const uint8_t JPEG_EOI = 0xD9;
const uint8_t JPEG_SOF0 = 0xC0;
const uint8_t JPEG_DHT = 0xC4;
const uint8_t JPEG_DQT = 0xDB;
const uint8_t JPEG_SOS = 0xDA;

class JPEGDecoder {
public:
    JPEGDecoder();
    ~JPEGDecoder();

    bool decode(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    BitReader* bitReader;
    JPEGParser* parser;
    std::vector<QuantizationTable> quantizationTables;
    std::vector<HuffmanTable> huffmanTables;
    bool readJPEGFile(const std::string& filePath);
    bool parseMarkers();
    bool decodeHuffmanData();
    bool performIDCT();
    bool convertColorSpace();
    bool writeBMPFile(const std::string& filePath);

};

#endif
