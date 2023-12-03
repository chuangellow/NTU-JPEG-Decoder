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
    currentMCU = MCU();
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

    int dcCoefficient = decodeDC(dcTree, component.getComponentID());

    std::vector<int> block(64, 0);
    block[0] = dcCoefficient;

    if (!decodeAC(acTree, block))
    {
        std::cerr << "Failed to decode AC coefficients" << std::endl;
        return false;
    }

    storeBlock(component.getComponentID(), mcuX, mcuY, block);

    return true;
}

int MCUDecoder::decodeDC(const std::shared_ptr<HuffmanNode> &dcTree, int componentID)
{
    int symbol = decodeSymbol(dcTree);
    if (symbol == -1)
    {
        std::cerr << "Error decoding DC coefficient" << std::endl;
        return -1;
    }

    int additionalBits = readBits(symbol);
    if (additionalBits == -1)
    {
        std::cerr << "Error reading additional bits for DC coefficient" << std::endl;
        return -1;
    }

    int dcCoefficient = extend(additionalBits, symbol);

    previousDCCoefficient[componentID] += dcCoefficient;

    return previousDCCoefficient[componentID];
}

bool MCUDecoder::decodeAC(const std::shared_ptr<HuffmanNode> &acTree, std::vector<int> &block)
{
    for (int k = 1; k < 64; ++k)
    {
        int symbol = decodeSymbol(acTree);
        if (symbol == 0x00)
        { // EOB
            break;
        }

        int runLength = symbol >> 4;
        int additionalBitsLength = symbol & 0x0F;

        k += runLength;

        if (k >= 64)
        {
            std::cerr << "Error: Run-length exceeded in AC coefficients" << std::endl;
            return false;
        }

        int additionalBits = readBits(additionalBitsLength);
        if (additionalBits == -1)
        {
            std::cerr << "Error reading additional bits for AC coefficient" << std::endl;
            return false;
        }

        block[k] = extend(additionalBits, additionalBitsLength);
    }

    return true;
}

int MCUDecoder::extend(int additionalBits, int size)
{
    if (size == 0)
    {
        return 0;
    }

    int maxValue = (1 << size) - 1;

    if (additionalBits > maxValue >> 1)
    {
        return additionalBits;
    }
    return additionalBits - maxValue;
}

void MCUDecoder::storeBlock(int componentId, int x, int y, const std::vector<int> &blockData)
{
    Block block;
    block.data = blockData;
    if (componentId == 1)
    {
        currentMCU.YBlocks.push_back(Block{block});
    }
    else if (componentId == 2)
    {
        currentMCU.CbBlocks.push_back(Block{block});
    }
    else if (componentId == 3)
    {
        currentMCU.CrBlocks.push_back(Block{block});
    }
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
            std::cerr << "Error reading bit" << std::endl;
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
                std::cout << "------ Component " << comp << " Block (" << y << ", " << x << ") ------\n";
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
        if (dataPos >= compressedData.size())
        {
            std::cerr << "Error: No more data to read" << std::endl;
            return -1;
        }

        bitBuffer = compressedData[dataPos++];

        if (bitBuffer == 0xFF && dataPos < compressedData.size() && compressedData[dataPos] == 0x00)
        {
            dataPos++;
        }

        bitCount = 8;
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