#include "RoadImage.h"

#include <list>
#include <cmath>

RoadImage::RoadImage() : image(Image(10000, 13000)) {}

void RoadImage::addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color) {
    std::optional<std::pair<size_t, size_t>> last = std::nullopt;
    for (const std::pair<double, double>& coord : path) {
        size_t row = -1 * round(coord.second * 2000.0) + 86000;
        size_t col = round(coord.first * 2000.0) + 184000;

        image.setPixel(row, col, {std::get<0>(color), std::get<1>(color), std::get<2>(color)});

        if (last) { // if there was a previous coordinate in list
            int row_diff = (int)(*last).first - (int)row;
            int col_diff = (int)(*last).second - (int)col;

            if (row_diff) {
                // (col) = m*(row) + b
                double m = (double)col_diff / (double)row_diff;
                double b = (double)col - m * (double)row;

                // resolve row diff
                if (row_diff > 0) { // if 
                    for (int i = 1; i < row_diff; i++) { // for each row diff assign a col
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                } else if (row_diff < 0) {
                    for (int i = -1; i > row_diff; i--) { // for each row diff assign a col
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
            }

            if (col_diff) {
                // (row) = m*(col) + b
                double m = (double)row_diff / (double)col_diff;
                double b = (double)row - m * (double)col;

                if (col_diff > 0) {
                    for (int i = 1; i < col_diff; i++) { // for each col diff assign a row
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
                else if (col_diff < 0) {
                    for (int i = -1; i > col_diff; i--) { // for each col diff assign a row
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
            }
        }
        last = {row, col};
    }
}

void RoadImage::addPath(Reader::RoadEntry road, std::tuple<uint8_t, uint8_t, uint8_t> color) {
    addPath(road.coordinates_list, color);
}

void RoadImage::addPaths(std::list<std::list<std::pair<double, double>>> paths, std::tuple<uint8_t, uint8_t, uint8_t> color) {
    for (const std::list<std::pair<double, double>>& path : paths) {
        addPath(path, color);
    }
}

void RoadImage::addPaths(std::list<Reader::RoadEntry> roads, std::tuple<uint8_t, uint8_t, uint8_t> color) {
    for (const Reader::RoadEntry& road : roads) {
        addPath(road, color);
    }
}

void RoadImage::toPNG(const std::string& file_name) {
    image.toPNG(file_name);
}
