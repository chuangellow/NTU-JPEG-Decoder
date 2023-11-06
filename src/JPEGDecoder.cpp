#include "JPEGDecoder.h"

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
