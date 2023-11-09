#include "HuffmanTable.h"

HuffmanTable::HuffmanTable(uint8_t identifier, uint8_t cls)
    : tableClass(cls), tableID(identifier) {
    for (int i = 0; i < 16; ++i) {
        codeLengths[i] = 0;
    }
}

void HuffmanTable::setCodes(const uint8_t lengths[16], const uint8_t* codes, size_t totalCodes) {
    for (int i = 0; i < 16; ++i) {
        codeLengths[i] = lengths[i];
    }

    symbols.assign(codes, codes + totalCodes);
}

std::vector<uint8_t> HuffmanTable::getSymbols() const {
    return symbols;
}

uint8_t HuffmanTable::getTableClass() const {
    return tableClass;
}

uint8_t HuffmanTable::getTableID() const {
    return tableID;
}

uint8_t HuffmanTable::getCodeLength(uint8_t symbol) const {
    return codeLengths[symbol];
}
