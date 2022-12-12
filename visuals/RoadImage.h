#pragma once

#include "image.h"
#include "../data/reader.h"

#include <list>

class RoadImage {
    public:
        RoadImage();
        RoadImage(const std::size_t& width, const std::size_t& height);
        void addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth);
        void addBoldPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth);
        void toPNG(const std::string& file_name) const;
    private:
        Image image;
        std::size_t width_;
        std::size_t height_;
};
