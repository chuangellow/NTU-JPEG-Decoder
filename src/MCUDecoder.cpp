#include "MCUDecoder.h"

MCUDecoder::MCUDecoder(const std::vector<uint8_t> &compressedData, FrameParameter &frameParam,
                       ScanParameter &scanParam, const std::vector<QuantizationTable> &quantizationTables,
                       const std::vector<std::shared_ptr<HuffmanNode>> &dcHuffmanTrees,
                       const std::vector<std::shared_ptr<HuffmanNode>> &acHuffmanTrees)
    : compressedData(compressedData), dataPos(0), bitBuffer(0), bitCount(0), frameParam(frameParam),
      scanParam(scanParam), quantizationTables(quantizationTables), dcHuffmanTrees(dcHuffmanTrees),
      acHuffmanTrees(acHuffmanTrees)
{
    previousDCCoefficient = std::vector<int>(frameParam.getComponents().size(), 0);
}

bool MCUDecoder::decodeMCU()
{
    for (auto &frameComponent : frameParam.getComponents())
    {
        int hSampling = (int)frameComponent.getHorizontalSamplingFactor();
        int vSampling = (int)frameComponent.getVerticalSamplingFactor();

        ScanComponent *scanComponent = findScanComponent(frameComponent.getComponentID());
        if (!scanComponent)
        {
            std::cerr << "Scan component not found for ID " << (int)frameComponent.getComponentID() << std::endl;
            return false;
        }

        for (int v = 0; v < vSampling; ++v)
        {
            for (int h = 0; h < hSampling; ++h)
            {
                if (!decodeBlock(*scanComponent))
                {
                    std::cerr << "Failed to decode block (" << v << ", " << h << ") for component " << (int)frameComponent.getComponentID() << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}

ScanComponent *MCUDecoder::findScanComponent(uint8_t componentID)
{
    auto &components = scanParam.getComponents();
    for (auto &scanComponent : components)
    {
        if (scanComponent.getComponentID() == componentID)
        {
            return &scanComponent;
        }
    }
    return nullptr;
}

bool MCUDecoder::decodeBlock(ScanComponent &component)
{
    int dcTableID = (int)component.getDCTableID();
    int acTableID = (int)component.getACTableID();

    auto &dcTree = dcHuffmanTrees[dcTableID];
    auto &acTree = acHuffmanTrees[acTableID];

    int dcCoefficient = decodeSymbol(dcTree);
    if (dcCoefficient == -1)
    {
        std::cerr << "Failed to decode DC coefficient" << std::endl;
        return false;
    }

    int componentID = component.getComponentID();
    dcCoefficient += previousDCCoefficient[componentID];
    previousDCCoefficient[componentID] = dcCoefficient;

    std::vector<int> acCoefficients(63, 0);
    for (int i = 0; i < 63; ++i)
    {
        int symbol = decodeSymbol(acTree);
        if (symbol == 0)
        {
            break;
        }

        int runLength = symbol >> 4;
        int size = symbol & 0xF;

        i += runLength;

        if (i >= 63)
        {
            std::cerr << "Run-length exceeds block size" << std::endl;
            return false;
        }

        int amplitude = readAmplitude(size);
        acCoefficients[i] = amplitude;
    }

    // Process the coefficients (e.g., inverse quantization, inverse DCT, etc.)
    // ...

    return true;
}

int MCUDecoder::readAmplitude(int size)
{
    if (size == 0)
    {
        return 0;
    }
    int amplitude = readBits(size);
    if (amplitude == -1)
    {
        return -1;
    }
    int mask = 1 << (size - 1);
    if (!(amplitude & mask))
    {
        amplitude -= (1 << size) - 1;
    }
    return amplitude;
}

int MCUDecoder::decodeSymbol(const std::shared_ptr<HuffmanNode> &tree)
{
    auto node = tree;
    if (node == nullptr)
    {
        std::cerr << "Huffman tree is null" << std::endl;
        return -1;
    }
    while (node && node->value == -1)
    {
        int bit = readBit();
        if (bit == -1)
        {
            return -1;
        }
        node = bit ? node->right : node->left;
    }
    return node ? node->value : -1;
}

int MCUDecoder::readBit()
{
    if (bitCount == 0)
    {
        if (dataPos < compressedData.size())
        {
            bitBuffer = compressedData[dataPos++];
            bitCount = 8;
        }
        else
        {
            return -1;
        }
    }

    int bit = (bitBuffer >> 7) & 1;
    bitBuffer <<= 1;
    --bitCount;

    return bit;
}

int MCUDecoder::readBits(int size)
{
    int value = 0;
    for (int i = 0; i < size; ++i)
    {
        int bit = readBit();
        if (bit == -1)
            return -1;
        value = (value << 1) | bit;
    }
    return value;
}