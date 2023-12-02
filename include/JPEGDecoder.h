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

class JPEGDecoder
{
public:
    JPEGDecoder();
    ~JPEGDecoder();

    bool decode(const std::string &inputFilePath, const std::string &outputFilePath);

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
    bool performIDCT();
    bool convertColorSpace();
    bool writeBMPFile(const std::string &filePath);
};

#endif
