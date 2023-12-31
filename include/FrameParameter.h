#ifndef FRMAEPARAMETER_H
#define FRMAEPARAMETER_H

#include <iostream>
#include <vector>

class FrameComponent
{
public:
    FrameComponent(uint8_t componentId, uint8_t horizontalSamplingFactor, uint8_t verticalSamplingFactor, uint8_t quantizationTableID);
    uint8_t getComponentID();
    uint8_t getHorizontalSamplingFactor();
    uint8_t getVerticalSamplingFactor();
    uint8_t getQuantizationTableID();
    void printComponentInfo();

private:
    uint8_t componentID;
    uint8_t horizontalSamplingFactor;
    uint8_t verticalSamplingFactor;
    uint8_t quantizationTableID;
};

class FrameParameter
{
public:
    FrameParameter();
    FrameParameter(uint8_t frameType, uint8_t precision, uint16_t height, uint16_t width, uint8_t numComponents, std::vector<FrameComponent> components);
    uint8_t getFrameType();
    uint8_t getPrecision();
    uint16_t getHeight();
    uint16_t getWidth();
    uint8_t getNumComponents();
    uint16_t getMaxHorizontalSampling();
    uint16_t getMaxVerticalSampling();
    std::vector<FrameComponent> getComponents();
    void printFrameParameter();

private:
    uint8_t frameType;
    uint8_t precision;
    uint16_t height;
    uint16_t width;
    uint8_t numComponents;
    std::vector<FrameComponent> components;
};

#endif // FRMAEPARAMETER_H
