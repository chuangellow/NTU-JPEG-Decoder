#ifndef BITREADER_H
#define BITREADER_H

#include <fstream>
#include <vector>
#include <iostream>

class BitReader
{
public:
    explicit BitReader(const std::string &filePath);
    ~BitReader();

    void close();
    bool is_open() const;
    int getBitsLeft() const;

    unsigned int readBit();
    int readBits(int n);
    unsigned char readByte();
    unsigned short readWord();

    void unreadBit(unsigned int bit);
    void unreadByte(unsigned char byte);

private:
    std::ifstream fileStream;
    unsigned char buffer;
    int bitsLeft;

    void fillBuffer();
};

#endif
