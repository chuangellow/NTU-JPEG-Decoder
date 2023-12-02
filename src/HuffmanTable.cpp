#include "HuffmanTable.h"

HuffmanTable::HuffmanTable(uint8_t identifier, uint8_t cls)
    : tableClass(cls), tableID(identifier)
{
    for (int i = 0; i < 16; ++i)
    {
        codeLengths[i] = 0;
    }
}

void HuffmanTable::setCodes(const uint8_t lengths[16], const uint8_t *codes, size_t totalCodes)
{
    for (int i = 0; i < 16; ++i)
    {
        codeLengths[i] = lengths[i];
    }

    symbols.assign(codes, codes + totalCodes);
}

std::vector<uint8_t> HuffmanTable::getSymbols() const
{
    return symbols;
}

uint8_t HuffmanTable::getTableClass() const
{
    return tableClass;
}

uint8_t HuffmanTable::getTableID() const
{
    return tableID;
}

uint8_t HuffmanTable::getCodeLength(uint8_t symbol) const
{
    return codeLengths[symbol];
}

std::shared_ptr<HuffmanNode> HuffmanTable::buildHuffmanTree(const HuffmanTable &table)
{
    auto root = std::make_shared<HuffmanNode>();
    int code = 0;
    size_t symbolIndex = 0;

    for (size_t len = 1; len <= 16; ++len)
    {
        for (size_t i = 0; i < table.codeLengths[len - 1]; ++i)
        {
            auto current = root;
            for (int j = len - 1; j >= 0; --j)
            {
                bool bit = (code >> j) & 1;
                if (bit)
                {
                    if (!current->right)
                        current->right = std::make_shared<HuffmanNode>();
                    current = current->right;
                }
                else
                {
                    if (!current->left)
                        current->left = std::make_shared<HuffmanNode>();
                    current = current->left;
                }
            }
            if (symbolIndex < table.symbols.size())
            {
                current->value = table.symbols[symbolIndex++];
            }
            code++;
        }
        code <<= 1;
    }

    return root;
}

void HuffmanTable::printTable()
{
    std::cout << "Table ID: " << (int)tableID << std::endl;
    std::cout << "Table Class: " << (int)tableClass << std::endl;
    std::cout << "Code Lengths: ";
    for (int i = 0; i < 16; ++i)
    {
        std::cout << (int)codeLengths[i] << " ";
    }
    std::cout << std::endl;
    std::vector<uint8_t> symbols = getSymbols();
    std::cout << "Symbols: ";
    for (auto symbol : symbols)
    {
        std::cout << (int)symbol << " ";
    }
    std::cout << std::endl;
    return;
}
