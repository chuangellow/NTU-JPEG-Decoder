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
