#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

class Image {
private:
    void load_file();
    void load_header();
    void check_header();
    unsigned int section_to_uint_little_endian(const std::vector<unsigned char>& num_str, int start, unsigned int end);
protected:
    std::string filename;

    // Header
    std::vector<char> raw_header;
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
    unsigned short padding;

    static const short mask_eigen[5]; // https://www.ipol.im/pub/art/2013/87/?utm_source=doi

    std::fstream image_stream;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    Image(const std::string & filename);
public:
    long load_time;
    long operation_time;
    long store_time;

    void copy();
    void histo();
    void mono();
    void gauss();

};