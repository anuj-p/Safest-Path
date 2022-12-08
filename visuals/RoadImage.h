#pragma once

#include "image.h"
#include "../data/reader.h"

#include <list>

class RoadImage {
    public:
        RoadImage();
        void addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void addBoldPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void toPNG(const std::string& file_name) const;
    private:
        Image image;
};
