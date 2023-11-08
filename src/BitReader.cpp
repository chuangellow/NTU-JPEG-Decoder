#include "BitReader.h"

BitReader::BitReader(const std::string& filePath)
    : fileStream(filePath, std::ios::binary), buffer(0), bitsLeft(0) {}

BitReader::~BitReader() {
    close();
}

void BitReader::close() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

bool BitReader::is_open() const {
    return fileStream.is_open();
}

void BitReader::fillBuffer() {
    if (fileStream.good() && bitsLeft == 0) {
        buffer = fileStream.get();
        bitsLeft = 8;
    }
}

unsigned int BitReader::readBit() {
    if (bitsLeft == 0) {
        fillBuffer();
    }
    unsigned int bit = (buffer >> (bitsLeft - 1)) & 1;
    bitsLeft--;
    return bit;
}

unsigned char BitReader::readByte() {
    unsigned char byte = 0;
    for (int i = 0; i < 8; i++) {
        byte = (byte << 1) | readBit();
    }
    return byte;
}

unsigned short BitReader::readWord() {
    unsigned short word = readByte();
    word = (word << 8) | readByte();
    return word;
}
