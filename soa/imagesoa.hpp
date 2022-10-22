#include <vector>
#include "common/image.hpp"

class ImageAos: public Image {
private:
    struct data {
        std::vector<unsigned char> blue;
        std::vector<unsigned char> green;
        std::vector<unsigned char> red;
    }
public:
    void copy();
    void histo();
    void mono();
    void gauss();
};