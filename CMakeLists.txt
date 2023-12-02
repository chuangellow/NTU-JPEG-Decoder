cmake_minimum_required(VERSION 3.10)

project(JPEGDecoder VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(JPEGDecoder src/main.cpp src/JPEGDecoder.cpp src/BitReader.cpp src/JPEGParser.cpp src/QuantizationTable.cpp src/HuffmanTable.cpp src/FrameParameter.cpp src/ScanParameter.cpp src/MCUDecoder.cpp)

# If you have other .cpp files or a library to include, you can add them like this:
# add_executable(JPEGDecoder main.cpp JPEGDecoder.cpp OtherFile1.cpp OtherFile2.cpp)

# If you have subdirectories with more CMakeLists.txt files, use this:
# add_subdirectory(subdirectory_name)

# If you need to include header files from a different directory, use this:
include_directories(${PROJECT_SOURCE_DIR}/include)

# If you are linking against external libraries, you might need to find them first and then link them:
# find_package(SomeLibrary REQUIRED)
# target_link_libraries(JPEGDecoder SomeLibrary)

# If you have your own library, for example, a libJPEGDecoder.a static library, you can link it like this:
# target_link_libraries(JPEGDecoder libJPEGDecoder)

# If you need to define any compile definitions, you can add them like this:
# target_compile_definitions(JPEGDecoder PRIVATE SOME_DEFINITION)

