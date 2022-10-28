#include <chrono>
#include <fstream>
#include <string>
#include <vector>

#ifndef IMAGE_H
#define IMAGE_H
class Image {
private:
  void load_file();
  void load_header();
  void check_header();
  unsigned int
  section_to_uint_little_endian(const std::vector<unsigned char> &num_str,
                                int start, unsigned int end);

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

  void start_timer();
  long time_difference();

  std::vector<int>
      mask_eigen; // https://www.ipol.im/pub/art/2013/87/?utm_source=doi
  std::vector < std::vector<int>> mask;

  std::fstream image_stream;

  std::chrono::time_point<std::chrono::high_resolution_clock> start;

  Image(const std::string &filename);

  uint8_t gamma_delinearization(const uint8_t blue_p, const uint8_t green_p,
                                const uint8_t red_p);

public:
  long load_time;
  long operation_time;
  long store_time;

  void copy();
  void histo();
  void mono();
  void gauss();
};
#endif