#include <string>
#include <fstream>
#include "soa/imagesoa.hpp"
#include <filesystem>
#include <cmath>
#include <chrono>
#include <vector>

/**
 * Constructor for ImageSoa
 */
ImageSoa::ImageSoa(const std::string &filename) : Image::Image(filename) {
    this->load_data();
    this->load_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - this->start).count();
}

/**
 * Loads bytes into object
 */
void ImageSoa::load_data() {
    const unsigned int width_bytes = this->width * 3 + this->padding;   // Image constructor loaded fstream on image_stream
    std::vector<char> buffer;                           // Buffer for the whole line
    buffer.reserve(width_bytes);
    this->data.blue.reserve(this->image_size);  // Reserves points
    this->data.green.reserve(this->image_size); // Reserves points
    this->data.red.reserve(this->image_size);   // Reserves points
    unsigned int seek = this->image_start;
    this->image_stream.seekg(seek, this->image_stream.beg);
    for(int i = 0; i < this->height; ++i) {
        this->image_stream.read(static_cast<char*>(buffer.data()), this->width * 3);
        seek += width_bytes;
        this->image_stream.seekg(seek, this->image_stream.beg);
        for(int j = 0; j < this->width * 3; j += 3) {
            this->data.blue.push_back(buffer[j]);
            this->data.green.push_back(buffer[j+1]);
            this->data.red.push_back(buffer[j+2]);
        }
    }
}

/**
 * Dumps image data into a new file
 */
void ImageSoa::store(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    std::ofstream out_bmp;
    out_bmp.open(out_dir.generic_string() + this->filename.substr(this->filename.find_last_of('/')), std::ios::out | std::ios::binary);
    out_bmp.seekp(0, this->image_stream.beg);
    out_bmp.write(static_cast<char *>(this->raw_header.data()), this->image_start);
    std::vector<char> buffer;                     // Buffer for the whole line
    buffer.reserve(this->width*3 + this->padding);
    for(int i=0; i < this->height; i++) {
        int j=0;
        for(; j < this->width; j++) {
            buffer[3*j] = this->data.blue[i*this->width + j];
            buffer[3*j+1] = this->data.green[i*this->width + j];
            buffer[3*j+2] = this->data.red[i*this->width + j];
        }
        out_bmp.write(buffer.data(), this->width*3);
        for(int k=0; k<this->padding; k++) out_bmp.put('\0'); // Adds padding
    }
    out_bmp.close();
    this->store_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
}

/**
 * Copies from one directory to another
 */
void ImageSoa::copy(const std::filesystem::path &out_dir) {
    this->store(out_dir);
}

/**
 * Generates histogram of each color and saves it into a .hst file
 */
void ImageSoa::histo(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    std::ofstream out_bmp;
    std::string filename = out_dir.generic_string() + this->filename.substr(this->filename.find_last_of('/'));
    filename.replace(filename.size() - 4, 4, "");
    filename += ".hst";
    out_bmp.open(filename, std::ios::out);
    out_bmp.seekp(0, this->image_stream.beg);
    unsigned int buffer[256 * 3] = {0x0}; // Histogram data
    for (unsigned int i = 0; i < this->data.blue.size(); ++i) { // Surjective function using value (mod 256) + offset
        buffer[this->data.red[i]] += 1;
        buffer[this->data.green[i] + 256] += 1;
        buffer[this->data.blue[i] + 512] += 1;
    }
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
    this->start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < 256 * 3; ++i) {
        std::string num = std::to_string(buffer[i]);
        out_bmp << num << std::endl;
    }
    this->store_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
}

/**
 * Applies colorimetric (perceptual luminance-preserving) conversion to grayscale
 */
void ImageSoa::mono(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < this->data.blue.size(); ++i) {
        double red = this->data.red[i] / 255.0; // Normalize red
        double green = this->data.green[i] / 255.0; // Normalize green
        double blue = this->data.blue[i] / 255.0; // Normalize blue
        if(red <=  0.04045) red = red / 12.92; // Red
        else red = std::pow(((red + 0.055)/1.055), 2.4);
        if(green <=  0.04045) green = green / 12.92; // Green
        else green = std::pow(((green + 0.055)/1.055), 2.4);
        if(blue <=  0.04045) blue = blue / 12.92; // Blue
        else blue = std::pow(((blue + 0.055)/1.055), 2.4);
        long double linear_luminance = 0.2126 * red + 0.7152 * green + 0.0722 * blue; // Adjusted luminance of pixel
        long double gamma_luminance = 0;
        if(linear_luminance <= 0.0031308) gamma_luminance = 12.92 * linear_luminance;
        else gamma_luminance = 1.055*pow(linear_luminance, 0.41666666666666666) - 0.055; // 1/2.4 = 0.41666666
        uint8_t grey_pixel = gamma_luminance*255; // Denormalize
        this->data.red[i] = grey_pixel;
        this->data.blue[i] = grey_pixel;
        this->data.green[i] = grey_pixel;
    }
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
    this->store(out_dir);
}


void ImageSoa::gaussian_mask_row(const int i, const int j) {
    unsigned long sum_blue = 0;
    unsigned long sum_green = 0;
    unsigned long sum_red = 0;
    for(int s= -2; s<=2; ++s) {
        if(0 <= (j+s) && (j+s) < this->width) { 
            int index = i*this->width + j+s;
            sum_blue += this->mask_eigen[s+2]*this->data.blue[index];
            sum_green += this->mask_eigen[s+2]*this->data.green[index];
            sum_red += this->mask_eigen[s+2]*this->data.red[index];
        }
    }
    sum_blue /= 17;
    sum_green /= 17;
    sum_red /= 17;
    this->data.blue[i*this->width + j] = sum_blue;
    this->data.green[i*this->width + j] = sum_green;
    this->data.red[i*this->width + j] = sum_red;
}

void ImageSoa::gaussian_mask_column(const int i, const int j) {
    unsigned long sum_blue = 0;
    unsigned long sum_green = 0;
    unsigned long sum_red = 0;
    for(int s= -2; s<=2; ++s) {
        if(0 <= (i+s) && (i+s) < this->height) { 
            int index = (i+s)*this->width + j;
            sum_blue += this->mask_eigen[s+2]*this->data.blue[index];
            sum_green += this->mask_eigen[s+2]*this->data.green[index];
            sum_red += this->mask_eigen[s+2]*this->data.red[index];
        }
    }
    sum_blue /= 17;
    sum_green /= 17;
    sum_red /= 17;
    this->data.blue[i*this->width + j] = sum_blue;
    this->data.green [i*this->width + j]= sum_green;
    this->data.red[i*this->width + j] = sum_red;
}

void ImageSoa::gaussian_mask(const int i, const int j){
    unsigned long sum_blue = 0;
    unsigned long sum_green = 0;
    unsigned long sum_red = 0;
    for(int s= -2; s<=2; ++s) {
        for(int t= -2; t<=2; ++t) {
            if(0 <= (i+s) && (i+s) < this->height &&
               0 <= (j+t) && (j+t) < this->width     ) {
                int index = (i+s)*this->width + (j+t);
                sum_blue += this->mask[s+2][t+2]*this->data.blue[index];
                sum_green += this->mask[s+2][t+2]*this->data.green[index];
                sum_red += this->mask[s+2][t+2]*this->data.red[index];
            }
        }
    }
    sum_blue /= 273;
    sum_green /= 273;
    sum_red /= 273;
    
    this->data.blue[i*this->width + j] = sum_blue;
    this->data.green[i*this->width + j] = sum_green;
    this->data.red[i*this->width + j] = sum_red;
}

void ImageSoa::gauss(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i < this->height; i++) {
        for(int j=0; j < this->width; j++) {
            this->gaussian_mask_row(i, j);    // Only the rows

        }
    }
    for(int j=0; j < this->width; j++) {
        for(int i=0; i < this->height; i++) {
            this->gaussian_mask_column(i, j); // Only the columns
        }
    }
    /* Inneficient version
    for(int j=0; j < this->width; j++) {
        for(int i=0; i < this->height; i++) {
            this->gaussian_mask(i, j);
        }
    }
    */
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
    this->store(out_dir);
}