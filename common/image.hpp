#include <string>
#include <fstream>
class Image {
private:
    void load_file();
    void load_header();
    unsigned int section_to_uint(const std::string& num, unsigned int start, const unsigned int end);
protected:
    std::string filename;
    unsigned int file_size;
    unsigned int image_start;
    unsigned int header_size;
    unsigned int width;
    unsigned int height;
    unsigned int image_size;
    std::fstream image_stream;

    Image(const std::string & filename);
public:
    virtual void copy();
    virtual void histo();
    virtual void mono();
    virtual void gauss();
};