#include <string>
#include <fstream>
#include "aos/imageaos.hpp"
#include <filesystem>

#include <iostream> // DEBUG

ImageAos::ImageAos(const std::string & filename) : Image(filename) {
    this->load_data();
}

void ImageAos::load_data() {
    // Image constructor loaded fstream on image_stream
    char buffer[this->width]; // Buffer for the whole line
    this->data.reserve(this->height*this->width);
    this->image_stream.seekg(this->image_start, this->image_stream.beg);
    for(unsigned int i=0; i<this->height; ++i) {
        this->image_stream.read(buffer, this->width);
        this->image_stream.seekg(static_cast<unsigned int>(this->image_stream.tellg()) + 
                                this->image_start + 
                                this->padding, 
                                this->image_stream.beg);
        for(unsigned int j=0; j<this->width; j+=3){
            pixel point;
            point.blue &= buffer[j];
            point.green &= buffer[j+1];
            point.red &= buffer[j+2];
            this->data.push_back(point);
        }
    }
}

/**
 * Dumps image data into a new file
 */
void ImageAos::store(const std::filesystem::path & out_dir) {
    std::ofstream out_bmp;
    out_bmp.open(out_dir.generic_string()+this->filename.substr(this->filename.find_last_of('/')), std::ios::out | std::ios::binary);
    out_bmp.seekp(0, this->image_stream.beg);
    out_bmp.write(reinterpret_cast<char*>(this->raw_header.data()), this->image_start);
    out_bmp.close();
}

void ImageAos::copy(const std::filesystem::path & out_dir){
    this->store(out_dir);
}
void ImageAos::histo(const std::filesystem::path & out_dir){}
void ImageAos::mono(const std::filesystem::path & out_dir){}
void ImageAos::gauss(const std::filesystem::path & out_dir){}