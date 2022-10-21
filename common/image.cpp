#include <fstream>
#include <stdexcept>
#include "image.hpp"

unsigned int Image::section_to_uint(const std::string& num_str, unsigned int start, const unsigned int end){
    unsigned int num = 0;
    while (start < end) {
       num = num * 10 + num_str[start++] - '0';
    }
    return num;
}

void Image::load_file(){
    this->image_stream.open(this->filename, std::ios::in | std::ios::binary);
}

void Image::load_header(){
    static char buffer[54];
    this->image_stream.read(buffer, 54);
    if(buffer[0] != 'B' || buffer[1] != 'M'){
        throw std::logic_error("File does not contain initial B or M characters");
    }
    this->file_size = this->section_to_uint(buffer, 2, 6);
    this->image_start = this->section_to_uint(buffer, 10, 14);
    this->header_size = this->section_to_uint(buffer, 14, 18);
    this->width = this->section_to_uint(buffer, 18, 22);
    this->height = this->section_to_uint(buffer, 22, 26);
    this->image_size = this->section_to_uint(buffer, 34, 38);
}

Image::Image(const std::string & filename){
    this->filename = filename;
    this->load_file();
    this->load_header();
}