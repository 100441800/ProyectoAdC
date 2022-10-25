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
    int width;
    int height;
    unsigned int planes;
    unsigned int point_size;
    unsigned int compression;
    int image_size;
    short padding;

    const short mask_eigen[5] = {1, 4, 7, 4, 1};
    const short mask[5][5] = {{1, 4, 7, 4, 1}, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1}};
    //const short mask_eigen[5]; // https://www.ipol.im/pub/art/2013/87/?utm_source=doi
    //const short mask[5][5];

    std::fstream image_stream;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    Image(const std::string & filename);
    
    uint8_t gamma_delinearization(const uint8_t blue, const uint8_t green, const uint8_t red);

public:
    long load_time;
    long operation_time;
    long store_time;

    void copy();
    void histo();
    void mono();
    void gauss();

};