#pragma once

#include "image.h"
#include "../data/reader.h"

#include <list>

class RoadImage {
    public:
        RoadImage();
        void addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void addPath(Reader::RoadEntry road, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void addPaths(std::list<std::list<std::pair<double, double>>> paths, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void addPaths(std::list<Reader::RoadEntry> roads, std::tuple<uint8_t, uint8_t, uint8_t> color);
        void toPNG(const std::string& file_name);
    private:
        Image image;
};
