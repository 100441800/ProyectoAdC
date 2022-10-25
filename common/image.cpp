#include <fstream>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <iostream> // DEBUG
#include "image.hpp"
#include <cmath>

/**
 * Converts array of uint8_t to uint_32t using bitwise and shift operations
 * WAY FASTER than stol and similar methods
 */
unsigned int Image::section_to_uint_little_endian(const std::vector<uint8_t>& num_str, const int start, const unsigned int end){
    unsigned int num = 0x00000000; // 32b
    for(short i=end-1; i>=start && i>=0; --i) {
        num <<= 8; // Makes space for 1 byte
        num |= num_str[i]; // Insert 1 byte
    }
    return num;
}

void Image::load_file() {
    this->image_stream.open(this->filename, std::ios::in | std::ios::binary);
}

void Image::check_header(){
    if(this->initial_charBM != 0x4d42) { // Initial characters B and M
        throw std::invalid_argument("[ERROR] File " + this->filename + " does not contain initial B or M characters");
    }
    this->image_stream.seekg(0, std::ios::end);
    if(this->image_stream.tellg() != this->file_size) { // Real vs header file size
        throw std::invalid_argument("[ERROR] File header states incorrect BMP file size");
    }
    if(this->image_size != ((this->width*3 + this->padding)*this->height)) { // Image size
        throw std::invalid_argument("[ERROR] File header states incorrect BMP image size. From header: " + 
            std::to_string(this->image_size) + ". From height and width calculations: " + 
            std::to_string((this->width*3 + this->padding)*this->height) + 
            ". Padding: " + std::to_string(this->padding));
    }
    if(this->planes != 1) {
        throw std::invalid_argument("[ERROR] Incorrect number of planes: " + std::to_string(this->planes) + ". Must be 1");
    }
    if(this->point_size != 24) {
        throw std::invalid_argument("[ERROR] Point size is:" + std::to_string(this->point_size) + "Must be 24 bits");
    }
    if(this->compression != 0) {
        throw std::invalid_argument("[ERROR] Compression is:" + std::to_string(this->compression) + "Must be 0");
    }
}

void Image::load_header(){
    char buffer[54]; // Suppose header is of fixed size 54
    this->image_stream.read(buffer, 54);
    const std::vector<uint8_t> data(buffer, buffer + 54); // To unsigned char
    this->initial_charBM = this->section_to_uint_little_endian(data, 0, 2);
    this->file_size = this->section_to_uint_little_endian(data, 2, 6);
    this->image_start = this->section_to_uint_little_endian(data, 10, 14);
    this->header_size = this->section_to_uint_little_endian(data, 14, 18);
    this->width = this->section_to_uint_little_endian(data, 18, 22);
    this->height = this->section_to_uint_little_endian(data, 22, 26);
    this->planes = this->section_to_uint_little_endian(data, 26, 28);
    this->point_size = this->section_to_uint_little_endian(data, 28, 30);
    this->compression = this->section_to_uint_little_endian(data, 30, 34);
    this->image_size = this->section_to_uint_little_endian(data, 34, 38);
    this->padding = (4 - (this->width * 3 % 4)) % 4;
    // Adds whole header
    this->raw_header.reserve(this->image_start);
    this->image_stream.seekg(0, this->image_stream.beg);
    this->image_stream.read(static_cast<char*>(this->raw_header.data()), this->image_start);

}

Image::Image(const std::string & filename) {
    this->load_time = 0;
    this->operation_time = 0;
    this->store_time = 0;
    this->start = std::chrono::high_resolution_clock::now();
    this->filename = filename;
    this->load_file();
    this->load_header();
    this->check_header();	
}