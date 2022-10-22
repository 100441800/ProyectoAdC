#include <string>
#include <fstream>
#include <vector>

class Image {
private:
    void load_file();
    void load_header();
    void check_header();
    unsigned int section_to_uint_little_endian(const std::vector<unsigned char>& num_str, int start, unsigned int end);
protected:
    std::string filename;
    unsigned int initial_charBM;
    unsigned int file_size;
    unsigned int image_start;
    unsigned int header_size;
    unsigned int width;
    unsigned int height;
    unsigned int planes;
    unsigned int point_size;
    unsigned int compression;
    unsigned int image_size;
    std::fstream image_stream;

    Image(const std::string & filename);
public:
    void copy();
    void histo();
    void mono();
    void gauss();
};