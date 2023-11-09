#ifndef HUFFMANTABLE_H
#define HUFFMANTABLE_H

#include <vector>
#include <cstdint>

class HuffmanTable {
public:
    HuffmanTable(uint8_t tableID, uint8_t tableClass);
    void setCodes(const uint8_t lengths[16], const uint8_t* codes, size_t totalCodes);
    std::vector<uint8_t> getSymbols() const;
    uint8_t getTableClass() const;
    uint8_t getTableID() const;
    uint8_t getCodeLength(uint8_t symbol) const;

private:
    uint8_t tableClass; // 0 = DC, 1 = AC
    uint8_t tableID; // 0-3
    uint8_t codeLengths[16];
    std::vector<uint8_t> symbols;
};

#endif // HUFFMANTABLE_H
