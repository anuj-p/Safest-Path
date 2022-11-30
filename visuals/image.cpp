#include "image.h"

Image::Image(unsigned int width, unsigned int height) : width_(width), height_(height) {
    image_ = new std::tuple<unsigned int, unsigned int, unsigned int>[width_ * height_];
}

std::tuple<unsigned int, unsigned int, unsigned int> Image::getPixel(unsigned int row, unsigned int col) {
    // TODO memory check
    return image_[row + col * row];
}

void Image::setPixel(unsigned int row, unsigned int col, std::tuple<unsigned int, unsigned int, unsigned int> val) {
    image_[row + col * row] = val;
}

void Image::toPPM() {
    // TODO
}

Image::~Image() {
    delete[] image_;
}