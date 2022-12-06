#pragma once

#include <tuple>
#include <string>

class Image {
    public:
        Image(size_t width, size_t height);
        std::tuple<uint8_t, uint8_t, uint8_t> getPixel(size_t row, size_t col);
        void setPixel(size_t row, size_t col, std::tuple<uint8_t, uint8_t, uint8_t> val);
        void toPPM(const std::string& file_name);
        void toPNG(const std::string& file_name);
        ~Image();
    private:
        std::tuple<uint8_t, uint8_t, uint8_t>* image_;
        size_t width_;
        size_t height_;
};
