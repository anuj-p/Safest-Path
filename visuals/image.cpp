#include "image.h"
#include "lodepng.h"

#include <algorithm>
#include <fstream>

Image::Image(size_t width, size_t height) : width_(width), height_(height) {
    image_ = new std::tuple<uint8_t, uint8_t, uint8_t>[width_ * height_];
    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            image_[col + row * width_] = {255, 255, 255};
        }
    }
}

std::tuple<uint8_t, uint8_t, uint8_t> Image::getPixel(size_t row, size_t col) {
    return image_[col + row * width_];
}

void Image::setPixel(size_t row, size_t col, std::tuple<uint8_t, uint8_t, uint8_t> val) {
    image_[col + row * width_] = val;
}

void Image::toPPM(const std::string& file_name) {
    std::ofstream file;
    file.open(file_name);
    if (file.bad() || file.fail()) throw std::runtime_error("");
    file << "P3" << std::endl;
    file << width_ << " " << height_ << std::endl;
    file << 255 << std::endl;
    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            std::tuple<uint8_t, uint8_t, uint8_t> pixel = getPixel(row, col);
            
            file << std::get<0>(pixel) << std::endl;
            file << std::get<1>(pixel) << std::endl;
            file << std::get<2>(pixel) << std::endl;
        }
    }
    file.close();
}

void Image::toPNG(const std::string& file_name) {
    unsigned char *byte_data = new unsigned char[width_ * height_ * 4];

    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            std::tuple<uint8_t, uint8_t, uint8_t> pixel = getPixel(row, col);

            size_t i = col + row * width_;
            byte_data[(i * 4)] = std::get<0>(pixel);
            byte_data[(i * 4) + 1] = std::get<1>(pixel);
            byte_data[(i * 4) + 2] = std::get<2>(pixel);
            byte_data[(i * 4) + 3] = 255;
        }
    }

    unsigned error = lodepng::encode(file_name, byte_data, width_, height_);
    if (error) throw std::runtime_error("");

    delete[] byte_data;
}

Image::~Image() {
    delete[] image_;
}
