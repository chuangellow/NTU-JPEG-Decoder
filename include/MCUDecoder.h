#ifndef MCUDECODER_H
#define MCUDECODER_H

#include "BitReader.h"
#include "HuffmanTable.h"
#include "FrameParameter.h"
#include "QuantizationTable.h"
#include "ScanParameter.h"
#include <vector>
#include <memory>

struct Block
{
    std::vector<double> data;
    Block() : data(64, 0) {}
};

struct MCU
{
    std::vector<Block> YBlocks;
    std::vector<Block> CbBlocks;
    std::vector<Block> CrBlocks;
};

class MCUDecoder
{
public:
    MCUDecoder(const std::vector<uint8_t> &compressedData, FrameParameter &frameParam,
               ScanParameter &scanParam, const std::vector<QuantizationTable> &quantizationTables,
               const std::vector<std::shared_ptr<HuffmanNode>> &dcHuffmanTrees,
               const std::vector<std::shared_ptr<HuffmanNode>> &acHuffmanTrees);

    bool decodeMCU();
    bool decodeBlock(ScanComponent &component, int mcuX, int mcuY);
    int decodeDC(const std::shared_ptr<HuffmanNode> &dcTree, int componentID);
    std::vector<int> decodeAC(const std::shared_ptr<HuffmanNode> &acTree);
    bool decodeAC(const std::shared_ptr<HuffmanNode> &acTree, std::vector<double> &block);
    int decodeSymbol(const std::shared_ptr<HuffmanNode> &tree);
    ScanComponent *findScanComponent(uint8_t componentID);
    int extend(int additionalBits, int size);
    void printDecodedBlocks() const;
    MCU currentMCU;
    MCU &getDecodedMCU() { return currentMCU; }

private:
    const std::vector<uint8_t> &compressedData;
    size_t dataPos;
    unsigned char bitBuffer;
    int bitCount;
    FrameParameter frameParam;
    ScanParameter scanParam;
    std::vector<QuantizationTable> quantizationTables;
    std::vector<std::shared_ptr<HuffmanNode>> dcHuffmanTrees;
    std::vector<std::shared_ptr<HuffmanNode>> acHuffmanTrees;
    std::vector<int> previousDCCoefficient;
    std::vector<std::vector<std::vector<std::vector<int>>>> decodedBlocks;
    void storeBlock(int componentId, int x, int y, const std::vector<double> &blockData);
    int readBit();
    int readBits(int size);
};

#endif // MCUDECODER_H
