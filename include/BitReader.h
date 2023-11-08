#ifndef BITREADER_H
#define BITREADER_H

#include <fstream>
#include <vector>
#include <iostream>

class BitReader {
public:
    explicit BitReader(const std::string& filePath);
    ~BitReader();

    void close();
    bool is_open() const;

    unsigned int readBit();
    unsigned char readByte();
    unsigned short readWord();

private:
    std::ifstream fileStream;
    unsigned char buffer;
    int bitsLeft;

    void fillBuffer();
};

#endif
