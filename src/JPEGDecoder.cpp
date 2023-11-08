#include "JPEGDecoder.h"

JPEGDecoder::JPEGDecoder() : bitReader(nullptr), parser(nullptr) {
}

JPEGDecoder::~JPEGDecoder() {
    if (bitReader != nullptr) {
        delete bitReader;
    }
    if (parser != nullptr) {
        delete parser;
    }
}

bool JPEGDecoder::decode(const std::string& inputFilePath, const std::string& outputFilePath) {
    if (!readJPEGFile(inputFilePath)) {
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
    bitReader = new BitReader(filePath);
    if (!bitReader->is_open()) {
        return false;
    }

    // Read SOI marker
    if (bitReader->readWord() != JPEG_SOI) {
        return false;
    }

    parser = new JPEGParser(*bitReader);
    if (!parseMarkers()) {
        return false;
    }
    return true;
}

bool JPEGDecoder::parseMarkers() {
    if (!parser) {
        return false;
    }
    uint16_t marker = 0;
    while ((marker = bitReader->readWord()) != JPEG_EOI) {
        switch (marker) {
            case JPEG_SOF0:
                if (!parser->parseSOF0()) {
                    return false;
                }
                break;
            case JPEG_DHT:
                if (!parser->parseDHT()) {
                    return false;
                }
                break;
            case JPEG_DQT: {
                auto tables = parser->parseDQT();
                quantizationTables.insert(quantizationTables.end(), tables.begin(), tables.end());
                if (quantizationTables.empty()) {
                    std::cerr << "No quantization tables found" << std::endl;
                    return false;
                }
                break;
            }
            case JPEG_SOS:
                if (!parser->parseSOS()) {
                    return false;
                }
                break;
            default:
                break;
        }
    }
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
