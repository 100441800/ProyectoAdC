#include <string>
#include <iostream>
#include "aos/imageaos.hpp"
#include <filesystem>

ImageAos::ImageAos(const std::string & filename) : Image(filename) {}

void ImageAos::load_data(){

}

void ImageAos::save(const std::filesystem::path & out_dir){

}

void ImageAos::copy(const std::filesystem::path & out_dir){}
void ImageAos::histo(const std::filesystem::path & out_dir){}
void ImageAos::mono(const std::filesystem::path & out_dir){}
void ImageAos::gauss(const std::filesystem::path & out_dir){}