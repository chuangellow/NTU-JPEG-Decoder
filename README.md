# NTU-JPEG-Decoder
Source code of JPEG-Decoder for ITCT course

## Prerequisites

To build and run this project, you will need:

- CMake (version 3.10 or higher)
- A C++ compiler that supports C++17

## Building the Project

To build the project, follow these steps:
1. Clone the repository to your local machine.
2. Navigate to the root directory of the project.
3. Create a new directory for the build:

```
$ cd ./r11944040-JPEG-Decoder
$ mkdir build
$ cd build
$ cmake ..
$ make JPEGDecoder
```

After a successful build, you should have the ```JPEGDecoder``` executable in the build directory.

## Running the JPEGDecoder

To decode a ```.jpg``` image to a ```.bmp``` image, use the following command:

```
./JPEGDecoder <input.jpg> <output.bmp>
```

## Automated Image Conversion

To automatically convert all ```.jpg``` images in the ```Image``` directory, you can use the provided shell script ```convert_images.sh```. This script will process each image and generate a corresponding .bmp file in the same directory.

First, ensure that the script is executable:

```
chmod +x convert_images.sh
```

Then run the script:

```
./convert_images.sh
```

## Debugging with JPEGDecoderDebug

The JPEGDecoderDebug executable is built with debug messages enabled. It prints detailed information about the decoding process of MCU(0, 0) at every stage, which can be useful for development and debugging purposes.

To build the debug version, run:

```
make JPEGDecoderDebug
```

Then, to run the debug version and see the debug messages, use:

```
./JPEGDecoderDebug <input.jpg> <output.bmp>
```
