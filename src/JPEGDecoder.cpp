#include "JPEGDecoder.h"
#include "BitReader.h"

JPEGDecoder::JPEGDecoder() {
}

JPEGDecoder::~JPEGDecoder() {
}

bool JPEGDecoder::decode(const std::string& inputFilePath, const std::string& outputFilePath) {
    if (!readJPEGFile(inputFilePath)) {
        return false;
    }

    if (!parseMarkers()) {
        return false;
    }

    if (!decodeHuffmanData()) {
        return false;
    }

    if (!performIDCT()) {
        return false;
    }

    if (!convertColorSpace()) {
        return false;
    }

    if (!writeBMPFile(outputFilePath)) {
        return false;
    }

    return true;
}

bool JPEGDecoder::readJPEGFile(const std::string& filePath) {
    BitReader bitReader(filePath);
    if (!bitReader.is_open()) {
        return false;
    }

    // Read SOI marker
    if (bitReader.readWord() != JPEG_SOI) {
        return false;
    }

    // Read markers
    while (true) {
        uint16_t marker = bitReader.readWord();
        uint16_t segmentLength = 0;
        switch (marker) {
            case JPEG_APP0:
                break;
            case JPEG_SOF0: // Baseline DCT
                segmentLength = bitReader.readWord();
                break;
            case JPEG_DHT: // Huffman table
                segmentLength = bitReader.readWord();
                break;
            case JPEG_DQT: // Quantization table
                segmentLength = bitReader.readWord();
                break;
            case JPEG_SOS: // Start of scan
                segmentLength = bitReader.readWord();
                break;
            case JPEG_EOI: // End of image
                segmentLength = bitReader.readWord();
                return true;
            default:
                break;
        }
    }
    return true;
}

bool JPEGDecoder::parseMarkers() {
    return true;
}

bool JPEGDecoder::decodeHuffmanData() {
    return true;
}

bool JPEGDecoder::performIDCT() {
    return true;
}

bool JPEGDecoder::convertColorSpace() {
    return true;
}

bool JPEGDecoder::writeBMPFile(const std::string& filePath) {
    return true;
}
