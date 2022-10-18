#!/bin/sh
cmake -S . -B image-aos -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-12 -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-12 -DCMAKE_BUILD_TYPE=Release
cmake -S . -B image-soa -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-12 -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-12 -DCMAKE_BUILD_TYPE=Release

cmake --build image-soa --target imgsoa
cmake --build image-aos --target imgaos
