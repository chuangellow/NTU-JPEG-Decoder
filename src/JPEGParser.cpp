#include "JPEGParser.h"
#include <iostream>

JPEGParser::JPEGParser(BitReader &reader) : bitReader(reader) {}

bool JPEGParser::parseAPP0()
{
    return true;
}

uint16_t JPEGParser::getLength()
{
    return bitReader.readWord() - 2;
}

FrameParameter JPEGParser::parseSOF0()
{
    uint16_t length = getLength();
    uint8_t precision = bitReader.readByte();
    uint16_t height = bitReader.readWord();
    uint16_t width = bitReader.readWord();
    uint8_t numComponents = bitReader.readByte();

    std::vector<FrameComponent> components;

    for (int i = 0; i < numComponents; ++i)
    {
        uint8_t componentID = bitReader.readByte();
        uint8_t samplingFactors = bitReader.readByte();
        uint8_t horizontalSamplingFactor = samplingFactors >> 4;
        uint8_t verticalSamplingFactor = samplingFactors & 0x0F;
        uint8_t quantizationTableID = bitReader.readByte();
        FrameComponent component(componentID, horizontalSamplingFactor, verticalSamplingFactor, quantizationTableID);
        components.push_back(component);
    }
    return FrameParameter(0, precision, height, width, numComponents, components);
}

std::vector<HuffmanTable> JPEGParser::parseDHT()
{
    std::vector<HuffmanTable> huffmanTables;
    uint16_t length = getLength();

    while (length > 0)
    {
        uint8_t huffmanInfo = bitReader.readByte();
        uint8_t tableClass = huffmanInfo >> 4;
        uint8_t tableID = huffmanInfo & 0x0F;

        HuffmanTable huffmanTable(tableID, tableClass);

        uint8_t codeLengths[16];
        uint8_t totalCodes = 0;
        for (int i = 0; i < 16; ++i)
        {
            codeLengths[i] = bitReader.readByte();
            totalCodes += codeLengths[i];
        }

        uint8_t codes[totalCodes];
        for (int i = 0; i < totalCodes; ++i)
        {
            codes[i] = bitReader.readByte();
        }

        huffmanTable.setCodes(codeLengths, codes, totalCodes);
        huffmanTables.push_back(huffmanTable);
        length -= 1 + 16 + totalCodes;
    }
    return huffmanTables;
}

std::vector<QuantizationTable> JPEGParser::parseDQT()
{
    std::vector<QuantizationTable> tables;
    uint16_t length = getLength();

    while (length > 0)
    {
        uint8_t qtInfo = bitReader.readByte();
        uint8_t precision = qtInfo >> 4;
        uint8_t tableID = qtInfo & 0x0F;

        QuantizationTable qt(tableID, precision);

        for (int i = 0; i < 64; ++i)
        {
            uint16_t qValue;
            if (precision == 0)
            {
                qValue = bitReader.readByte();
            }
            else
            {
                qValue = bitReader.readWord();
            }
            qt.setValue(i, qValue);
        }
        tables.push_back(qt);
        length -= (precision == 0) ? (1 + 64) : (1 + 64 * 2);
    }
    return tables;
}

ScanParameter JPEGParser::parseSOS()
{
    uint16_t length = getLength();
    uint8_t numComponents = bitReader.readByte();
    std::vector<ScanComponent> components;
    for (int i = 0; i < numComponents; ++i)
    {
        uint8_t componentID = bitReader.readByte();
        uint8_t huffmanTableIDs = bitReader.readByte();
        uint8_t dcTableID = huffmanTableIDs >> 4;
        uint8_t acTableID = huffmanTableIDs & 0x0F;
        ScanComponent component(componentID, dcTableID, acTableID);
        components.push_back(component);
    }
    length -= 1 + numComponents * 2;
    while (length > 0)
    {
        bitReader.readByte();
        length -= 1;
    }

    return ScanParameter(numComponents, components);
}

std::vector<uint8_t> JPEGParser::parseScanData()
{
    std::vector<uint8_t> scanData;
    uint8_t byteData = bitReader.readByte();
    bool isEOI = true;
    while (isEOI)
    {
        if (byteData == JPEG_PREFIX)
        {
            uint8_t nextByteData = bitReader.readByte();
            if (nextByteData == JPEG_DATA)
            {
                scanData.push_back(byteData);
                scanData.push_back(nextByteData);
                byteData = bitReader.readByte();
            }
            else if (nextByteData == JPEG_EOI)
            {
                isEOI = false;
            }
        }
        else
        {
            scanData.push_back(byteData);
            byteData = bitReader.readByte();
        }
    }
    return scanData;
}
