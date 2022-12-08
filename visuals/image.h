#pragma once

#include <tuple>
#include <string>

class Image {
    public:
        Image(const size_t& width, const size_t& height);
        std::tuple<uint8_t, uint8_t, uint8_t> getPixel(const size_t& row, const size_t& col) const;
        void setPixel(const size_t& row, const size_t& col, const std::tuple<uint8_t, uint8_t, uint8_t>& val);
        void toPPM(const std::string& file_name) const;
        void toPNG(const std::string& file_name) const;
        ~Image();
    private:
        std::tuple<uint8_t, uint8_t, uint8_t>* image_;
        size_t width_;
        size_t height_;
};
