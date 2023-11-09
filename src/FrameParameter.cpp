#include "FrameParameter.h"

FrameParameter::FrameParameter(uint8_t frameType, uint8_t precision, uint16_t height, uint16_t width, uint8_t numComponents, std::vector<Component> components)
    : frameType(frameType), precision(precision), height(height), width(width), numComponents(numComponents), components(components) {
}

Component::Component(uint8_t id, uint8_t horizontalSamplingFactor, uint8_t verticalSamplingFactor, uint8_t quantizationTableID)
    : componentID(id), horizontalSamplingFactor(horizontalSamplingFactor), verticalSamplingFactor(verticalSamplingFactor), quantizationTableID(quantizationTableID) {
}

uint8_t FrameParameter::getFrameType() {
    return frameType;
}

uint8_t FrameParameter::getPrecision() {
    return precision;
}

uint16_t FrameParameter::getHeight() {
    return height;
}

uint16_t FrameParameter::getWidth() {
    return width;
}

uint8_t FrameParameter::getNumComponents() {
    return numComponents;
}

std::vector<Component> FrameParameter::getComponents() {
    return components;
}
