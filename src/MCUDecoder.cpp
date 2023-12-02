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
        frameComponent.printComponentInfo();
        if (!scanComponent)
        {
            std::cerr << "Scan component not found for ID " << (int)frameComponent.getComponentID() << std::endl;
            return false;
        }

        for (int v = 0; v < vSampling; ++v)
        {
            for (int h = 0; h < hSampling; ++h)
            {
                if (!decodeBlock(*scanComponent, h, v))
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

bool MCUDecoder::decodeBlock(ScanComponent &component, int mcuX, int mcuY)
{
    int dcTableID = component.getDCTableID();
    int acTableID = component.getACTableID();

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

    std::vector<int> block(64, 0);
    block[0] = dcCoefficient;

    for (int i = 1; i < 64; ++i)
    {
        int symbol = decodeSymbol(acTree);
        if (symbol == 0)
        { // EOB
            break;
        }

        int runLength = symbol >> 4;
        int size = symbol & 0xF;

        i += runLength;
        if (i >= 64)
        {
            std::cerr << "Run-length exceeds block size" << std::endl;
            return false;
        }

        int amplitude = readAmplitude(size);
        block[i] = amplitude;
    }

    // Store the block
    storeBlock(componentID, mcuX, mcuY, block);
    return true;
}

void MCUDecoder::storeBlock(int componentId, int x, int y, const std::vector<int> &block)
{
    // Resize for component
    if (decodedBlocks.size() <= componentId)
    {
        decodedBlocks.resize(componentId + 1);
    }

    // Resize for y-coordinate
    if (decodedBlocks[componentId].size() <= y)
    {
        decodedBlocks[componentId].resize(y + 1);
    }

    // Resize for x-coordinate
    if (decodedBlocks[componentId][y].size() <= x)
    {
        decodedBlocks[componentId][y].resize(x + 1);
    }

    // Store the block
    decodedBlocks[componentId][y][x] = block;
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

void MCUDecoder::printDecodedBlocks() const
{
    for (size_t comp = 0; comp < decodedBlocks.size(); ++comp)
    {
        for (size_t y = 0; y < decodedBlocks[comp].size(); ++y)
        {
            for (size_t x = 0; x < decodedBlocks[comp][y].size(); ++x)
            {
                std::cout << "------ Component " << comp << " Block (" << x << ", " << y << ") ------\n";
                const auto &block = decodedBlocks[comp][y][x];
                for (int i = 0; i < 64; i++)
                {
                    if (i > 0 && i % 8 == 0)
                        std::cout << "\n";
                    std::cout << block[i] << " ";
                }
                std::cout << "\n";
            }
        }
    }
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