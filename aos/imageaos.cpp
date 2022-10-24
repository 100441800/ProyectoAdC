#include <string>
#include <fstream>
#include "aos/imageaos.hpp"
#include <filesystem>
#include <cmath>
#include <chrono>

ImageAos::ImageAos(const std::string &filename) : Image(filename) {
    this->load_data();
    this->load_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
}

void ImageAos::load_data() {
    unsigned int width_bytes = this->width * 3; // Image constructor loaded fstream on image_stream
    char buffer[width_bytes];             // Buffer for the whole line
    this->data.reserve(this->image_size); // Reserves points (each point 3 Bytes)
    unsigned int seek = this->image_start;
    this->image_stream.seekg(seek, this->image_stream.beg);
    for (unsigned int i = 0; i < this->height; ++i)
    {
        this->image_stream.read(buffer, this->width * 3);
        seek += width_bytes;
        this->image_stream.seekg(seek, this->image_stream.beg);
        for (unsigned int j = 0; j < this->width * 3; j += 3)
        {
            pixel point;
            point.blue = buffer[j];
            point.green = buffer[j + 1];
            point.red = buffer[j + 2];
            this->data.push_back(point);
        }
    }
}

/**
 * Dumps image data into a new file
 */
void ImageAos::store(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    std::ofstream out_bmp;
    out_bmp.open(out_dir.generic_string() + this->filename.substr(this->filename.find_last_of('/')), std::ios::out | std::ios::binary);
    out_bmp.seekp(0, this->image_stream.beg);
    out_bmp.write(static_cast<char *>(this->raw_header.data()), this->image_start);
    for (unsigned int i = 0, j = 0; i < this->data.size(); ++i, j += 3) {
        out_bmp.put(this->data[i].blue);
        out_bmp.put(this->data[i].green);
        out_bmp.put(this->data[i].red);
    }
    out_bmp.close();
    this->store_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
}

void ImageAos::copy(const std::filesystem::path &out_dir) {
    this->store(out_dir);
}

void ImageAos::histo(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    std::ofstream out_bmp;
    std::string filename = out_dir.generic_string() + this->filename.substr(this->filename.find_last_of('/'));
    filename.replace(filename.size() - 4, 4, "");
    filename += ".hst";
    out_bmp.open(filename, std::ios::out);
    out_bmp.seekp(0, this->image_stream.beg);
    unsigned int buffer[256 * 3] = {0x0}; // Histogram data number
    for (unsigned int i = 0; i < this->data.size(); ++i) { // Surjective function
        pixel point = this->data[i];
        buffer[(unsigned char)point.red % 256] += 1;
        buffer[((unsigned char)point.green % 256) + 256] += 1;
        buffer[((unsigned char)point.blue % 256) + 512] += 1;
    }
    for (unsigned int i = 0; i < 256 * 3; ++i) {
        std::string num = std::to_string(buffer[i]);
        out_bmp << num << std::endl;
    }
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
}
void ImageAos::mono(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < this->data.size(); ++i){
        pixel point = this->data[i];
        double red = point.red / 255.0;
        double green = point.green / 255.0;
        double blue = point.blue / 255.0;
        if(red <=  0.04045) red = red / 12.92; // Red
        else red = std::pow(((red + 0.055)/1.055), 2.4);
        if(green <=  0.04045) green = green / 12.92; // Green
        else green = std::pow(((green + 0.055)/1.055), 2.4);
        if(blue <=  0.04045) blue = blue / 12.92; // Blue
        else blue = std::pow(((blue + 0.055)/1.055), 2.4);
        
        long double linear_luminance = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
        long double gamma_luminance = 0;
        if(linear_luminance <= 0.0031308) gamma_luminance = 12.92 * linear_luminance;
        else gamma_luminance = 1.055*pow(linear_luminance, 0.41666666666666666) - 0.055; // 1/2.4 = 0.41666666
        
        uint8_t grey_pixel = gamma_luminance*255;
        this->data[i].red = grey_pixel;
        this->data[i].blue = grey_pixel;
        this->data[i].green = grey_pixel;
    }
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
    this->store(out_dir);
}

void ImageAos::gaussian_mask_row(const int i, const int j) {
    unsigned long sum_blue = 0;
    unsigned long sum_green = 0;
    unsigned long sum_red = 0;
    for(int s= -2; s<=2; ++s) {
        if(0 <= (j+s) && (j+s) < this->width) [[LIKELY]] {
            int index = i*this->width + j+s;
            sum_blue += this->mask_eigen[s+2]*this->data[index].blue;
            sum_green += this->mask_eigen[s+2]*this->data[index].green;
            sum_red += this->mask_eigen[s+2]*this->data[index].red;
        }
    }
    sum_blue /= 17;
    sum_green /= 17;
    sum_red /= 17;
    this->data[i*this->width + j].blue = sum_blue;
    this->data[i*this->width + j].green = sum_green;
    this->data[i*this->width + j].red = sum_red;
}

void ImageAos::gaussian_mask_column(const int i, const int j) {
    unsigned long sum_blue = 0;
    unsigned long sum_green = 0;
    unsigned long sum_red = 0;
    for(int s= -2; s<=2; ++s) {
        if(0 <= (i+s) && (i+s) < this->height) [[LIKELY]] {
            int index = (i+s)*this->width + j;
            sum_blue += this->mask_eigen[s+2]*this->data[index].blue;
            sum_green += this->mask_eigen[s+2]*this->data[index].green;
            sum_red += this->mask_eigen[s+2]*this->data[index].red;
        }
    }
    sum_blue /= 17;
    sum_green /= 17;
    sum_red /= 17;
    this->data[i*this->width + j].blue = sum_blue;
    this->data[i*this->width + j].green = sum_green;
    this->data[i*this->width + j].red = sum_red;
}

void ImageAos::gauss(const std::filesystem::path &out_dir) {
    this->start = std::chrono::high_resolution_clock::now();
    // Only the rows
    for(int i=0; i < this->height; i++) {
        for(int j=0; j < this->width; j++) {
            this->gaussian_mask_row(i, j);
        }
    }
    // Only the columns
    for(int j=0; j < this->width; j++) {
        for(int i=0; i < this->height; i++) {
            this->gaussian_mask_column(i, j);
        }
    }
    this->operation_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - this->start).count();
    this->store(out_dir);
}