#!/bin/bash

cd build

for image in ../Image/*.jpg; do
    filename=$(basename -- "$image")
    filename="${filename%.*}"

    ./JPEGDecoder "$image" "../Image/$filename.bmp"
    echo "Converted $image to $filename.bmp"
done

echo "All images have been converted."
