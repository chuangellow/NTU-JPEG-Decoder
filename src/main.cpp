#include <iostream>
#include "JPEGDecoder.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.jpg> <output.bmp>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];

    JPEGDecoder decoder;

    bool success = decoder.decode(inputFilePath, outputFilePath);

    if (!success)
    {
        std::cerr << "Failed to decode the image." << std::endl;
        return 1;
    }

    std::cout << "Image successfully decoded and saved to " << outputFilePath << std::endl;

    return 0;
}
