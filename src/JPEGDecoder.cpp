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
        std::cerr << "Failed to read JPEG file" << std::endl;
        return false;
    }

    if (!decodeHuffmanData()) {
        std::cerr << "Failed to decode Huffman data" << std::endl;
        return false;
    }

    if (!performIDCT()) {
        std::cerr << "Failed to perform IDCT" << std::endl;
        return false;
    }

    if (!convertColorSpace()) {
        std::cerr << "Failed to convert color space" << std::endl;
        return false;
    }

    if (!writeBMPFile(outputFilePath)) {
        std::cerr << "Failed to write BMP file" << std::endl;
        return false;
    }

    return true;
}

bool JPEGDecoder::readJPEGFile(const std::string& filePath) {
    bitReader = new BitReader(filePath);
    if (!bitReader->is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    parser = new JPEGParser(*bitReader);
    if (!parseMarkers()) {
        std::cerr << "Failed to parse markers" << std::endl;
        return false;
    }
    return true;
}

bool JPEGDecoder::parseMarkers() {
    if (!parser) {
        std::cerr << "Parser is not initialized" << std::endl;
        return false;
    }
    uint8_t marker = 0;
    bool flag = true;
    while (flag) {
        marker = parser->bitReader.readByte();
        if (marker != JPEG_PREFIX) continue;
        marker = parser->bitReader.readByte();
        switch (marker) {
            case JPEG_SOI:
                break;
            case JPEG_SOF0:
                if (!parser->parseSOF0()) {
                    return false;
                }
                break;
            case JPEG_DHT: {
                auto tables = parser->parseDHT();
                huffmanTables.insert(huffmanTables.end(), tables.begin(), tables.end());
                if (huffmanTables.empty()) {
                    std::cerr << "No Huffman tables found" << std::endl;
                    return false;
                }
                break;
            }
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
            case JPEG_EOI:
                flag = false;
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
