#!/bin/bash
#SBATCH -o ./config.out
#SBATCH -e ./config.out

# Clean
mv image-aos/imgaos.cpp ./
mv image-soa/imgsoa.cpp ./
rm -rf image-aos
rm -rf image-soa

# Build directories
mkdir -p image-aos
mkdir -p image-soa

# Move back mains
mv imgaos.cpp ./image-aos
mv imgsoa.cpp ./image-soa

if [ $# -eq 1 ] && [ $1 == avignon ]; then
# Config build directories for Avignon
	cmake -S . -B image-aos -DCMAKE_CXX_COMPILER=/opt/gcc-12.1.0/bin/g++ -DCMAKE_BUILD_TYPE=Release
	cmake -S . -B image-soa -DCMAKE_CXX_COMPILER=/opt/gcc-12.1.0/bin/g++ -DCMAKE_BUILD_TYPE=Release
elif [ $# -eq 1 ] && [ $1 == macos ]; then
# Config build directories for MacOS
	find . -name ".DS_Store" -delete
	cmake -S . -B image-aos -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-12 -DCMAKE_BUILD_TYPE=Debug
	cmake -S . -B image-soa -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-12 -DCMAKE_BUILD_TYPE=Debug
else
	echo "Incorrect arguments: use macos or avignon"
	exit -1
fi

# Build targets on independent directories
cmake --build image-soa --target imgsoa --clean-first
cmake --build image-aos --target imgaos --clean-first