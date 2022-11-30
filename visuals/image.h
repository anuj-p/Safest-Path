#pragma once

#include <tuple>

class Image {
    public:
        Image(unsigned int width, unsigned int height);
        std::tuple<unsigned int, unsigned int, unsigned int> getPixel(unsigned int row, unsigned int col);
        void setPixel(unsigned int row, unsigned int col, std::tuple<unsigned int, unsigned int, unsigned int> val);
        void toPPM();
        ~Image();
    private:
        std::tuple<unsigned int, unsigned int, unsigned int>* image_;
        unsigned int width_;
        unsigned int height_;
};
