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
        std::cerr << "Failed to decode Huffman data" << std::endl;
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
    printCompressedData();
    return true;
}

void JPEGDecoder::printCompressedData()
{
    std::cout << "Compressed data: ";
    for (auto &data : compressedData)
    {
        std::cout << std::hex << (int)data << " ";
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
        else
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

    int mcuCountX = (frameParameter.getWidth() + 7) / 8;
    int mcuCountY = (frameParameter.getHeight() + 7) / 8;

    for (int y = 0; y < mcuCountY; ++y)
    {
        for (int x = 0; x < mcuCountX; ++x)
        {
            if (!mcuDecoder.decodeMCU())
            {
                std::cerr << "Failed to decode MCU at (" << x << ", " << y << ")" << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool JPEGDecoder::performIDCT()
{
    return true;
}

bool JPEGDecoder::convertColorSpace()
{
    return true;
}

bool JPEGDecoder::writeBMPFile(const std::string &filePath)
{
    return true;
}
