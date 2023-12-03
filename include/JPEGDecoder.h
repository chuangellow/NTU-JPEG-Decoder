#ifndef JPEGDECODER_H
#define JPEGDECODER_H

#include <string>
#include <iostream>
#include "JPEGMarker.h"
#include "BitReader.h"
#include "JPEGParser.h"
#include "QuantizationTable.h"
#include "HuffmanTable.h"
#include "FrameParameter.h"
#include "ScanParameter.h"
#include "MCUDecoder.h"

static const int ZigZagOrder[64] = {
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63};

struct Color
{
    double r, g, b;

    Color() : r(0), g(0), b(0) {}
    Color(double red, double green, double blue) : r(red), g(green), b(blue) {}
};

class JPEGDecoder
{
public:
    JPEGDecoder();
    ~JPEGDecoder();

    bool decode(const std::string &inputFilePath, const std::string &outputFilePath);
    std::vector<MCU> mcus;
    std::vector<Color> rgbData;

private:
    BitReader *bitReader;
    JPEGParser *parser;
    FrameParameter frameParameter;
    ScanParameter scanParameter;
    std::vector<uint8_t> compressedData;
    std::vector<QuantizationTable> quantizationTables;
    std::vector<HuffmanTable> huffmanTables;
    std::vector<std::shared_ptr<HuffmanNode>> dcHuffmanTrees;
    std::vector<std::shared_ptr<HuffmanNode>> acHuffmanTrees;
    bool readJPEGFile(const std::string &filePath);
    bool parseMarkers();
    void printMarkers();
    void printCompressedData();
    void buildHuffmanTrees();
    void printHuffmanTreePaths(const std::shared_ptr<HuffmanNode> &node, const std::string &path);
    void printAllHuffmanTreePaths();
    bool decodeHuffmanData();
    const std::vector<MCU> &getDecodedMCUs() const { return mcus; }
    void printDecodedMCU(int mcuX, int mcuY, int mcuCountX) const;
    void printBlock(const Block &block) const;
    bool performDequantization();
    void dequantizeBlock(Block &block, const QuantizationTable &qTable);
    bool performInverseZigZag();
    void inverseZigZag(Block &block);
    bool performIDCT();
    void applyIDCT(Block &block);
    bool performInverseSubsampling();
    std::vector<Block> upsampleComponents(const std::vector<Block> &components);
    Block upsampleBlock(const Block &block);
    Color convertYCbCrToRGB(double Y, double Cb, double Cr);
    bool convertColorSpace();
    bool writeBMPFile(const std::string &filePath);
    Color getPixelData(int x, int y);
};

#endif
