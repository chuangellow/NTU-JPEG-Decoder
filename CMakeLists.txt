cmake_minimum_required(VERSION 3.10)

project(JPEGDecoder VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(JPEGDecoder src/main.cpp src/JPEGDecoder.cpp src/BitReader.cpp src/JPEGParser.cpp src/QuantizationTable.cpp src/HuffmanTable.cpp src/FrameParameter.cpp src/ScanParameter.cpp src/MCUDecoder.cpp)

add_executable(JPEGDecoderDebug src/main.cpp src/JPEGDecoder.cpp src/BitReader.cpp src/JPEGParser.cpp src/QuantizationTable.cpp src/HuffmanTable.cpp src/FrameParameter.cpp src/ScanParameter.cpp src/MCUDecoder.cpp)

target_compile_definitions(JPEGDecoderDebug PRIVATE DEBUG)
