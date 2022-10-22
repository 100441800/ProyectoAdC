#include <vector>
#include <string>
#include <filesystem>
#include "common/image.hpp"

struct pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class ImageAos: public Image {
private:
    std::vector<pixel> data;
    void load_data();
    void store(const std::filesystem::path & out_dir);
public:
    ImageAos(const std::string & filename);
    void copy(const std::filesystem::path & out_dir);
    void histo(const std::filesystem::path & out_dir);
    void mono(const std::filesystem::path & out_dir);
    void gauss(const std::filesystem::path & out_dir);
};