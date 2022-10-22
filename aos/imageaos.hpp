#include <vector>
#include <string>
#include "common/image.hpp"

struct pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

class ImageAos: public Image {
private:
    std::vector<pixel> data;
public:
    ImageAos(const std::string & filename);
    void copy();
    void histo();
    void mono();
    void gauss();
};