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
$ mkdir build
$ cd build
$ cmake ..
$ make
```

After a successful build, you should have the JPEGDecoder executable in the build directory.

## Running the Decoder

To decode a ```.jpg``` image to a ```.bmp``` image, use the following command:

```
./JPEGDecoder <input.jpg> <output.bmp>
```
