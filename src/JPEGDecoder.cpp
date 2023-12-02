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
    return true;
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

void JPEGDecoder::printHuffmanTree(const std::shared_ptr<HuffmanNode> &node, int level)
{
    if (!node)
    {
        return;
    }
    for (int i = 0; i < level; ++i)
    {
        std::cout << "  ";
    }

    if (node->value != -1)
    {
        std::cout << "Value: " << node->value << "\n";
    }
    else
    {
        std::cout << "Node\n";
    }
    printHuffmanTree(node->left, level + 1);
    printHuffmanTree(node->right, level + 1);
}

void JPEGDecoder::printHuffmanTrees()
{
    for (int i = 0; i < dcHuffmanTrees.size(); ++i)
    {
        std::cout << "DC Huffman Tree " << i << "\n";
        printHuffmanTree(dcHuffmanTrees[i]);
    }
    for (int i = 0; i < acHuffmanTrees.size(); ++i)
    {
        std::cout << "AC Huffman Tree " << i << "\n";
        printHuffmanTree(acHuffmanTrees[i]);
    }
}

int JPEGDecoder::decodeSymbol(BitReader &reader, const std::shared_ptr<HuffmanNode> &tree)
{
    auto node = tree;
    while (node && node->value == -1)
    {
        if (reader.readBit() == 0)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return node ? node->value : -1;
}

bool JPEGDecoder::decodeHuffmanData()
{
    buildHuffmanTrees();
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
