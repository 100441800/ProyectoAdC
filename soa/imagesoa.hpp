#include <vector>
#include <string>
#include <filesystem>
#include "common/image.hpp"

#ifndef IMAGESOA_H
#define IMAGEAOS_H
struct pixels {
    std::vector<uint8_t> blue;
    std::vector<uint8_t> green;
    std::vector<uint8_t> red;
};

class ImageSoa: public Image {
private:
    pixels data;
    void load_data();
    void store(const std::filesystem::path & out_dir);
    void gaussian_mask(const int i, const int j);
    void gaussian_mask_row(const int i, const int j);
    void gaussian_mask_column(const int i, const int j);

public:
    ImageSoa(const std::string & filename);
    void copy(const std::filesystem::path & out_dir);
    void histo(const std::filesystem::path & out_dir);
    void mono(const std::filesystem::path & out_dir);
    void gauss(const std::filesystem::path & out_dir);
};
#endif