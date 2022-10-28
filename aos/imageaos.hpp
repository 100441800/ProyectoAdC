#include <vector>
#include <string>
#include <filesystem>

#include "common/image.hpp"

#ifndef IMAGEAOS_H
#define IMAGEAOS_H

struct pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

class ImageAos: public Image {
private:
    std::vector<pixel> data;
    void load_data();
    void store(const std::filesystem::path & out_dir);
    void gaussian_mask(const int i, const int j);
    void gaussian_mask_row(const int i, const int j);
    void gaussian_mask_column(const int i, const int j);

public:
    ImageAos(const std::string & filename);
    void copy(const std::filesystem::path & out_dir);
    void histo(const std::filesystem::path & out_dir);
    void mono(const std::filesystem::path & out_dir);
    void gauss(const std::filesystem::path & out_dir);
};
#endif