#include "ScanParameter.h"

ScanParameter::ScanParameter()
{
    numComponents = 0;
    components = std::vector<ScanComponent>();
}

ScanComponent::ScanComponent(uint8_t componentID, uint8_t dcTableID, uint8_t acTableID)
    : componentID(componentID), dcTableID(dcTableID), acTableID(acTableID)
{
}

ScanParameter::ScanParameter(uint8_t numComponents, std::vector<ScanComponent> components)
    : numComponents(numComponents), components(components)
{
}

uint8_t ScanComponent::getComponentID()
{
    return componentID;
}

uint8_t ScanComponent::getDCTableID()
{
    return dcTableID;
}

uint8_t ScanComponent::getACTableID()
{
    return acTableID;
}

void ScanComponent::printComponentInfo()
{
    std::cout << "Scan Component Info: " << std::endl;
    std::cout << "Component ID: " << (int)componentID << std::endl;
    std::cout << "DC Table ID: " << (int)dcTableID << std::endl;
    std::cout << "AC Table ID: " << (int)acTableID << std::endl;
}

uint8_t ScanParameter::getNumComponents()
{
    return numComponents;
}

std::vector<ScanComponent> ScanParameter::getComponents()
{
    return components;
}

void ScanParameter::printScanParameter()
{
    std::cout << "Number of Components: " << (int)numComponents << std::endl;
    for (int i = 0; i < numComponents; i++)
    {
        std::cout << "Component " << i << std::endl;
        components[i].printComponentInfo();
    }
}
