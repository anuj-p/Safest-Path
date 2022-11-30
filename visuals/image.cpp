#include "image.h"

#include <fstream>

Image::Image(unsigned int width, unsigned int height) : width_(width), height_(height) {
    image_ = new std::tuple<unsigned int, unsigned int, unsigned int>[width_ * height_]; // may have to set all to 0
}

std::tuple<unsigned int, unsigned int, unsigned int> Image::getPixel(unsigned int row, unsigned int col) {
    // TODO memory check
    return image_[row + col * row];
}

void Image::setPixel(unsigned int row, unsigned int col, std::tuple<unsigned int, unsigned int, unsigned int> val) {
    image_[row + col * row] = val;
}

void Image::toPPM(std::string file_name) {
    std::ofstream file;
    file.open(file_name);
    if (file.bad() || file.fail()) throw std::runtime_error("");
    file << "P3" << std::endl;
    file << width_ << " " << height_ << std::endl;
    file << 255 << std::endl;
    for (unsigned int row = 0; row < height_; row++) {
        for (unsigned int col = 0; col < width_; col++) {
            std::tuple<unsigned int, unsigned int, unsigned int> pixel = getPixel(row, col);
            
            file << std::get<0>(pixel) << std::endl;
            file << std::get<1>(pixel) << std::endl;
            file << std::get<2>(pixel) << std::endl;
        }
    }
    file.close();
}

Image::~Image() {
    delete[] image_;
}
