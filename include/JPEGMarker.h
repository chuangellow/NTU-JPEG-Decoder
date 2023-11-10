#ifndef JPEGMARKER_H
#define JPEGMARKER_H

#include <stdint.h>

const uint8_t JPEG_PREFIX = 0xFF;
const uint8_t JPEG_APP0 = 0xE0;
const uint8_t JPEG_SOI = 0xD8;
const uint8_t JPEG_EOI = 0xD9;
const uint8_t JPEG_SOF0 = 0xC0;
const uint8_t JPEG_DHT = 0xC4;
const uint8_t JPEG_DQT = 0xDB;
const uint8_t JPEG_SOS = 0xDA;

#endif // JPEGMARKER_H
