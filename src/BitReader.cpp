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

int BitReader::getBitsLeft() const {
    return bitsLeft;
}

void BitReader::fillBuffer() {
    if (fileStream.good() && bitsLeft == 0) {
        buffer = fileStream.get();
        bitsLeft = 8;
    }
    else {
        buffer = 0;
        bitsLeft = 0;
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

void BitReader::unreadBit(unsigned int bit) {
    buffer = (buffer << 1) | bit;
    bitsLeft++;
}

void BitReader::unreadByte(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        unreadBit((byte >> i) & 1);
    }
}
