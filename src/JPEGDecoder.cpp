#include "JPEGDecoder.h"

JPEGDecoder::JPEGDecoder() : bitReader(nullptr), parser(nullptr)
{
}

JPEGDecoder::~JPEGDecoder()
{
    if (bitReader != nullptr)
    {
        delete bitReader;
    }
    if (parser != nullptr)
    {
        delete parser;
    }
}

bool JPEGDecoder::decode(const std::string &inputFilePath, const std::string &outputFilePath)
{
    if (!readJPEGFile(inputFilePath))
    {
        std::cerr << "Failed to read JPEG file" << std::endl;
        return false;
    }

    if (!decodeHuffmanData())
    {
        std::cerr << "Failed to decode data" << std::endl;
        return false;
    }

    if (!performDequantization())
    {
        std::cerr << "Failed to perform dequantization" << std::endl;
        return false;
    }

    if (!performInverseZigZag())
    {
        std::cerr << "Failed to perform inverse zig-zag" << std::endl;
        return false;
    }

    if (!performIDCT())
    {
        std::cerr << "Failed to perform IDCT" << std::endl;
        return false;
    }

    if (!convertColorSpace())
    {
        std::cerr << "Failed to convert color space" << std::endl;
        return false;
    }

    if (!writeBMPFile(outputFilePath))
    {
        std::cerr << "Failed to write BMP file" << std::endl;
        return false;
    }

    return true;
}

bool JPEGDecoder::readJPEGFile(const std::string &filePath)
{
    bitReader = new BitReader(filePath);
    if (!bitReader->is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    parser = new JPEGParser(*bitReader);
    if (!parseMarkers())
    {
        std::cerr << "Failed to parse markers" << std::endl;
        return false;
    }
    return true;
}

bool JPEGDecoder::parseMarkers()
{
    if (!parser)
    {
        std::cerr << "Parser is not initialized" << std::endl;
        return false;
    }
    uint8_t marker = 0;
    bool flag = true;
    while (flag)
    {
        marker = parser->bitReader.readByte();
        if (marker != JPEG_PREFIX)
            continue;
        marker = parser->bitReader.readByte();
        switch (marker)
        {
        case JPEG_SOI:
            break;
        case JPEG_SOF0:
        {
            frameParameter = parser->parseSOF0();
            break;
        }
        case JPEG_DHT:
        {
            auto tables = parser->parseDHT();
            huffmanTables.insert(huffmanTables.end(), tables.begin(), tables.end());
            if (huffmanTables.empty())
            {
                std::cerr << "No Huffman tables found" << std::endl;
                return false;
            }
            break;
        }
        case JPEG_DQT:
        {
            auto tables = parser->parseDQT();
            quantizationTables.insert(quantizationTables.end(), tables.begin(), tables.end());
            if (quantizationTables.empty())
            {
                std::cerr << "No quantization tables found" << std::endl;
                return false;
            }
            break;
        }
        case JPEG_SOS:
        {
            scanParameter = parser->parseSOS();
            compressedData = parser->parseScanData();
            flag = false; // Check EOI marker in the parseScanData()
            break;
        }
        default:
            break;
        }
    }
    return true;
}

void JPEGDecoder::printMarkers()
{
    std::cout << "SOF0:" << std::endl;
    frameParameter.printFrameParameter();
    std::cout << std::endl;

    std::cout << "DHT:" << std::endl;
    printAllHuffmanTreePaths();

    std::cout << "DQT:" << std::endl;
    for (int i = 0; i < quantizationTables.size(); ++i)
    {
        std::cout << "Table " << i << ":" << std::endl;
        quantizationTables[i].printTable();
    }

    std::cout << "SOS:" << std::endl;
    scanParameter.printScanParameter();
    std::cout << std::endl;
}

void JPEGDecoder::printCompressedData()
{
    std::cout << "Compressed data: " << std::endl;
    for (auto &data : compressedData)
    {
        std::cout << std::hex << (int)data << std::endl;
    }
    std::cout << std::endl;
}

void JPEGDecoder::buildHuffmanTrees()
{
    for (const auto &table : huffmanTables)
    {
        auto tree = HuffmanTable::buildHuffmanTree(table);
        if (table.getTableClass() == 0)
        {
            dcHuffmanTrees.push_back(tree);
        }
        else if (table.getTableClass() == 1)
        {
            acHuffmanTrees.push_back(tree);
        }
    }
}

void JPEGDecoder::printHuffmanTreePaths(const std::shared_ptr<HuffmanNode> &node, const std::string &path)
{
    if (!node)
        return;

    if (node->value != -1)
    {
        std::cout << path << " 0x" << std::hex << node->value << std::dec << std::endl;
    }
    else
    {
        printHuffmanTreePaths(node->left, path + "0");
        printHuffmanTreePaths(node->right, path + "1");
    }
}

void JPEGDecoder::printAllHuffmanTreePaths()
{
    std::cout << "DC Huffman Trees:" << std::endl;
    for (auto &tree : dcHuffmanTrees)
    {
        printHuffmanTreePaths(tree, "");
    }

    std::cout << "\nAC Huffman Trees:" << std::endl;
    for (auto &tree : acHuffmanTrees)
    {
        printHuffmanTreePaths(tree, "");
    }
}

bool JPEGDecoder::decodeHuffmanData()
{
    buildHuffmanTrees();

    MCUDecoder mcuDecoder(compressedData, frameParameter, scanParameter, quantizationTables, dcHuffmanTrees, acHuffmanTrees);

    int mcuCountX = (frameParameter.getWidth() - 1) / (8 * frameParameter.getMaxHorizontalSampling()) + 1;
    int mcuCountY = (frameParameter.getHeight() - 1) / (8 * frameParameter.getMaxVerticalSampling()) + 1;

    std::cout << "MCU count: " << mcuCountX << " x " << mcuCountY << std::endl;

    for (int y = 0; y < mcuCountY; ++y)
    {
        for (int x = 0; x < mcuCountX; ++x)
        {
            if (!mcuDecoder.decodeMCU())
            {
                std::cerr << "Failed to decode MCU at (" << y << ", " << x << ")" << std::endl;
                return false;
            }
            mcus.push_back(mcuDecoder.getDecodedMCU());
        }
    }
    printDecodedMCU(0, 0, mcuCountX);
    return true;
}

void JPEGDecoder::printDecodedMCU(int mcuX, int mcuY, int mcuCountX) const
{
    int mcuIndex = mcuY * mcuCountX + mcuX;

    if (mcuIndex >= mcus.size())
    {
        std::cerr << "MCU index out of range." << std::endl;
        return;
    }

    const auto &mcu = mcus[mcuIndex];
    std::cout << "MCU(" << mcuY << ", " << mcuX << ")\n";

    for (size_t i = 0; i < mcu.YBlocks.size(); ++i)
    {
        std::cout << "------ Y: " << i << " ------\n";
        printBlock(mcu.YBlocks[i]);
    }

    for (size_t i = 0; i < mcu.CbBlocks.size(); ++i)
    {
        std::cout << "------ Cb: " << i << " ------\n";
        printBlock(mcu.CbBlocks[i]);
    }

    for (size_t i = 0; i < mcu.CrBlocks.size(); ++i)
    {
        std::cout << "------ Cr: " << i << " ------\n";
        printBlock(mcu.CrBlocks[i]);
    }
}

void JPEGDecoder::printBlock(const Block &block) const
{
    for (int i = 0; i < 64; i++)
    {
        if (i > 0 && i % 8 == 0)
        {
            std::cout << "\n";
        }
        std::cout << block.data[i] << " ";
    }
    std::cout << "\n";
}

bool JPEGDecoder::performDequantization()
{
    for (MCU &mcu : mcus)
    {
        for (Block &block : mcu.YBlocks)
        {
            dequantizeBlock(block, quantizationTables[frameParameter.getComponents()[0].getQuantizationTableID()]);
        }
        for (Block &block : mcu.CbBlocks)
        {
            dequantizeBlock(block, quantizationTables[frameParameter.getComponents()[1].getQuantizationTableID()]);
        }
        for (Block &block : mcu.CrBlocks)
        {
            dequantizeBlock(block, quantizationTables[frameParameter.getComponents()[2].getQuantizationTableID()]);
        }
    }
    std::cout << "After dequantization:" << std::endl;
    printDecodedMCU(0, 0, (frameParameter.getWidth() - 1) / (8 * frameParameter.getMaxHorizontalSampling()) + 1);
    return true;
}

void JPEGDecoder::dequantizeBlock(Block &block, const QuantizationTable &qTable)
{
    for (int i = 0; i < 64; ++i)
    {
        block.data[i] = static_cast<double>(block.data[i]) * qTable.getValue(i);
    }
}

bool JPEGDecoder::performInverseZigZag()
{
    for (MCU &mcu : mcus)
    {
        for (Block &block : mcu.YBlocks)
        {
            inverseZigZag(block);
        }
        for (Block &block : mcu.CbBlocks)
        {
            inverseZigZag(block);
        }
        for (Block &block : mcu.CrBlocks)
        {
            inverseZigZag(block);
        }
    }
    std::cout << "After inverse Zig-Zag:" << std::endl;
    printDecodedMCU(0, 0, (frameParameter.getWidth() - 1) / (8 * frameParameter.getMaxHorizontalSampling()) + 1);
    return true;
}

void JPEGDecoder::inverseZigZag(Block &block)
{
    std::vector<double> originalBlock(64, 0.0);
    for (int i = 0; i < 64; ++i)
    {
        originalBlock[i] = block.data[ZigZagOrder[i]];
    }
    block.data = originalBlock;
}

bool JPEGDecoder::performIDCT()
{
    for (MCU &mcu : mcus)
    {
        for (Block &block : mcu.YBlocks)
        {
            applyIDCT(block);
        }
        for (Block &block : mcu.CbBlocks)
        {
            applyIDCT(block);
        }
        for (Block &block : mcu.CrBlocks)
        {
            applyIDCT(block);
        }
    }
    std::cout << "After IDCT:" << std::endl;
    printDecodedMCU(0, 0, (frameParameter.getWidth() - 1) / (8 * frameParameter.getMaxHorizontalSampling()) + 1);
    return true;
}

void JPEGDecoder::applyIDCT(Block &block)
{
    std::vector<double> tempBlock(64, 0.0);

    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            double sum = 0.0;
            for (int u = 0; u < 8; ++u)
            {
                for (int v = 0; v < 8; ++v)
                {
                    double Cu = (u == 0) ? 0.707 : 1.0;
                    double Cv = (v == 0) ? 0.707 : 1.0;
                    sum += Cu * Cv * block.data[u * 8 + v] *
                           cos((2 * x + 1) * u * M_PI / 16) *
                           cos((2 * y + 1) * v * M_PI / 16);
                }
            }
            tempBlock[x * 8 + y] = static_cast<double>(sum / 4.0);
        }
    }

    block.data = tempBlock;
}

bool JPEGDecoder::convertColorSpace()
{
    return true;
}

bool JPEGDecoder::writeBMPFile(const std::string &filePath)
{
    return true;
}
