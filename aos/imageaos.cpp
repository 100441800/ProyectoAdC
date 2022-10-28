
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "aos/imageaos.hpp"
#include "common/image.hpp"
/**
 * Constructor for ImageAos
 */
ImageAos::ImageAos(const std::string &filename) : Image::Image(filename) {
  this->load_data();
  this->load_time = this->time_difference();
}

/**
 * Loads bytes into object
 */
void ImageAos::load_data() {
  const unsigned int width_bytes =
      this->width * 3 +
      this->padding;        // Image constructor loaded fstream on image_stream
  std::vector<char> buffer; // Buffer for the whole line
  buffer.reserve(width_bytes);
  this->data.reserve(this->image_size); // Reserves points (each point 3 Bytes)
  unsigned int seek = this->image_start;
  this->image_stream.seekg(seek, this->image_stream.beg);
  for (int i = 0; i < this->height; ++i) {
    this->image_stream.read(static_cast<char *>(buffer.data()),
                            this->width * 3);
    seek += width_bytes;
    this->image_stream.seekg(seek, this->image_stream.beg);
    for (int j = 0; j < this->width * 3; j += 3) {
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
  this->start_timer();
  std::ofstream out_bmp;
  out_bmp.open(out_dir.generic_string() +
                   this->filename.substr(this->filename.find_last_of('/')),
               std::ios::out | std::ios::binary);
  out_bmp.seekp(0, this->image_stream.beg);
  out_bmp.write(static_cast<char *>(this->raw_header.data()),
                this->image_start);
  std::vector<char> buffer; // Buffer for the whole line
  buffer.reserve(this->width * 3 + this->padding);
  for (int i = 0; i < this->height; i++) {
    for (int j = 0; j < this->width; j++) {
      buffer[3 * j] = this->data[i * this->width + j].blue;
      buffer[3 * j + 1] = this->data[i * this->width + j].green;
      buffer[3 * j + 2] = this->data[i * this->width + j].red;
    }
    out_bmp.write(buffer.data(), this->width * 3);
    for (int k = 0; k < this->padding; k++)
      out_bmp.put('\0'); // Adds padding
  }
  out_bmp.close();
  this->store_time = this->time_difference();
}

/**
 * Copies from one directory to anotherF
 */
void ImageAos::copy(const std::filesystem::path &out_dir) {
  this->store(out_dir);
}

/**
 * Generates histogram of each color and saves it into a .hst file
 */
void ImageAos::histo(const std::filesystem::path &out_dir) {
  this->start_timer();
  std::ofstream out_bmp;
  std::string filename =
      out_dir.generic_string() +
      this->filename.substr(this->filename.find_last_of('/'));
  filename.replace(filename.size() - 4, 4, "");
  filename += ".hst";
  out_bmp.open(filename, std::ios::out);
  out_bmp.seekp(0, this->image_stream.beg);
  unsigned int buffer[256 * 3] = {0x0}; // Histogram data
  for (unsigned int i = 0; i < this->data.size();
       ++i) { // Surjective function using value (mod 256) + offset
    pixel point = this->data[i];
    buffer[point.red] += 1;
    buffer[point.green + 256] += 1;
    buffer[point.blue + 512] += 1;
  }
  this->operation_time = this->time_difference();
  this->start_timer();
  for (unsigned int i = 0; i < 256 * 3; ++i) {
    std::string num = std::to_string(buffer[i]);
    out_bmp << num << std::endl;
  }
  this->store_time = this->time_difference();
}

/**
 * Applies colorimetric (perceptual luminance-preserving) conversion to
 * grayscale
 */
void ImageAos::mono(const std::filesystem::path &out_dir) {
  this->start_timer();
  for (unsigned int i = 0; i < this->data.size(); ++i) {
    uint8_t grey_pixel = this->gamma_delinearization(this->data[i].blue, this->data[i].green, this->data[i].red);
    this->data[i].red = grey_pixel;
    this->data[i].blue = grey_pixel;
    this->data[i].green = grey_pixel;
  }
  this->operation_time = this->time_difference();
  this->store(out_dir);
}

void ImageAos::gaussian_mask_row(const int i, const int j) {
  double sum_blue = 0;
  double sum_green = 0;
  double sum_red = 0;
  for (int s = -2; s <= 2; ++s) {
    if (0 <= (j + s) && (j + s) < this->width) {
      const int index = i * this->width + j + s;
      sum_blue += this->mask_eigen[s + 2] * this->data[index].blue;
      sum_green += this->mask_eigen[s + 2] * this->data[index].green;
      sum_red += this->mask_eigen[s + 2] * this->data[index].red;
    }
  }
  sum_blue /= 17.0;
  sum_green /= 17.0;
  sum_red /= 17.0;
  this->data[i * this->width + j].blue = std::round(sum_blue);
  this->data[i * this->width + j].green = std::round(sum_green);
  this->data[i * this->width + j].red = std::round(sum_red);
}

void ImageAos::gaussian_mask_column(const int i, const int j) {
  double sum_blue = 0;
  double sum_green = 0;
  double sum_red = 0;
  int index = 0;
  for (int s = -2; s <= 2; ++s) {
    if (0 <= (i + s) && (i + s) < this->height) {
      index = (i + s) * this->width + j;
      sum_blue += this->mask_eigen[s + 2] * this->data[index].blue;
      sum_green += this->mask_eigen[s + 2] * this->data[index].green;
      sum_red += this->mask_eigen[s + 2] * this->data[index].red;
    }
  }
  sum_blue /= 17.0;
  sum_green /= 17.0;
  sum_red /= 17.0;
  this->data[i * this->width + j].blue = std::round(sum_blue);
  this->data[i * this->width + j].green = std::round(sum_green);
  this->data[i * this->width + j].red = std::round(sum_red);
}


void ImageAos::gauss(const std::filesystem::path &out_dir) {
  this->start_timer();
  for (int i = 0; i < this->height; i++) {
    for (int j = 0; j < this->width; j++) {
      this->gaussian_mask_row(i, j); // Only the rows
    }
  }
  for (int j = 0; j < this->width; j++) {
    for (int i = 0; i < this->height; i++) {
      this->gaussian_mask_column(i, j); // Only the columns
    }
  }
  this->operation_time = this->time_difference();
  this->store(out_dir);
}