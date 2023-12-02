#include "FrameParameter.h"

FrameParameter::FrameParameter()
{
    frameType = -1;
    precision = 0;
    height = 0;
    width = 0;
    numComponents = 0;
    components = std::vector<FrameComponent>();
}

FrameParameter::FrameParameter(uint8_t frameType, uint8_t precision, uint16_t height, uint16_t width, uint8_t numComponents, std::vector<FrameComponent> components)
    : frameType(frameType), precision(precision), height(height), width(width), numComponents(numComponents), components(components)
{
}

FrameComponent::FrameComponent(uint8_t id, uint8_t horizontalSamplingFactor, uint8_t verticalSamplingFactor, uint8_t quantizationTableID)
    : componentID(id), horizontalSamplingFactor(horizontalSamplingFactor), verticalSamplingFactor(verticalSamplingFactor), quantizationTableID(quantizationTableID)
{
}

uint8_t FrameComponent::getComponentID()
{
    return componentID;
}

uint8_t FrameComponent::getHorizontalSamplingFactor()
{
    return horizontalSamplingFactor;
}

uint8_t FrameComponent::getVerticalSamplingFactor()
{
    return verticalSamplingFactor;
}

uint8_t FrameComponent::getQuantizationTableID()
{
    return quantizationTableID;
}

void FrameComponent::printComponentInfo()
{
    std::cout << "Frame Component Info:" << std::endl;
    std::cout << "Component ID: " << (int)componentID << std::endl;
    std::cout << "Horizontal Sampling Factor: " << (int)horizontalSamplingFactor << std::endl;
    std::cout << "Vertical Sampling Factor: " << (int)verticalSamplingFactor << std::endl;
    std::cout << "Quantization Table ID: " << (int)quantizationTableID << std::endl;
    return;
}

uint8_t FrameParameter::getFrameType()
{
    return frameType;
}

uint8_t FrameParameter::getPrecision()
{
    return precision;
}

uint16_t FrameParameter::getHeight()
{
    return height;
}

uint16_t FrameParameter::getWidth()
{
    return width;
}

uint8_t FrameParameter::getNumComponents()
{
    return numComponents;
}

void FrameParameter::printFrameParameter()
{
    std::cout << "Image Info:" << std::endl;
    std::cout << "Frame Type: " << (int)frameType << std::endl;
    std::cout << "Precision: " << (int)precision << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Number of Components: " << (int)numComponents << std::endl;
    for (int i = 0; i < numComponents; i++)
    {
        components[i].printComponentInfo();
    }
    return;
}

std::vector<FrameComponent> FrameParameter::getComponents()
{
    return components;
}
