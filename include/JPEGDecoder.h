#ifndef JPEGDECODER_H
#define JPEGDECODER_H

#include <string>
#include <iostream>

const uint16_t JPEG_APP0 = 0xFFE0;
const uint16_t JPEG_SOI = 0xFFD8;
const uint16_t JPEG_EOI = 0xFFD9;
const uint16_t JPEG_SOF0 = 0xFFC0;
const uint16_t JPEG_DHT = 0xFFC4;
const uint16_t JPEG_DQT = 0xFFDB;
const uint16_t JPEG_SOS = 0xFFDA;

class JPEGDecoder {
public:
    JPEGDecoder();
    ~JPEGDecoder();

    bool decode(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    bool readJPEGFile(const std::string& filePath);
    bool parseMarkers();
    bool decodeHuffmanData();
    bool performIDCT();
    bool convertColorSpace();
    bool writeBMPFile(const std::string& filePath);

};

#endif
