#ifndef SCANPARAMETER_H
#define SCANPARAMETER_H

#include <iostream>
#include <vector>

class ScanComponent
{
public:
    ScanComponent(uint8_t componentID, uint8_t dcTableID, uint8_t acTableID);
    uint8_t getComponentID();
    uint8_t getDCTableID();
    uint8_t getACTableID();
    void printComponentInfo();

private:
    uint8_t componentID;
    uint8_t dcTableID;
    uint8_t acTableID;
};

class ScanParameter
{
public:
    ScanParameter();
    ScanParameter(uint8_t numComponents, std::vector<ScanComponent> components);
    uint8_t getNumComponents();
    std::vector<ScanComponent> getComponents();
    void printScanParameter();

private:
    uint8_t numComponents;
    std::vector<ScanComponent> components;
};

#endif // SCANPARAMETER_H
