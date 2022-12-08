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
                if (row_diff > 0) {
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

void RoadImage::addBoldPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color) {
    std::optional<std::pair<size_t, size_t>> last = std::nullopt;
    for (const std::pair<double, double>& coord : path) {
        size_t row = -1 * round(coord.second * 2000.0) + 86000;
        size_t col = round(coord.first * 2000.0) + 184000;
        
        // image.setPixel(row, col, {std::get<0>(color), std::get<1>(color), std::get<2>(color)});

        if (last) { // if there was a previous coordinate in list
            int row_diff = (int)(*last).first - (int)row;
            int col_diff = (int)(*last).second - (int)col;

            if (row_diff) {
                // (col) = m*(row) + b
                double m = (double)col_diff / (double)row_diff;
                double b = (double)col - m * (double)row;

                // resolve row diff
                if (row_diff > 0) {
                    for (int i = 0; i < row_diff; i++) { // for each row diff assign a col
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b - 2), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b - 1), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b + 1), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b + 2), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                } else if (row_diff < 0) {
                    for (int i = 0; i > row_diff; i--) { // for each row diff assign a col
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b - 2), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b - 1), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b + 1), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)((int)row + i), (size_t)round(m*(double)((int)row + i) + b + 2), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
            }

            if (col_diff) {
                // (row) = m*(col) + b
                double m = (double)row_diff / (double)col_diff;
                double b = (double)row - m * (double)col;

                if (col_diff > 0) {
                    for (int i = 0; i < col_diff; i++) { // for each col diff assign a row
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b - 2), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b - 1), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b + 1), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b + 2), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
                else if (col_diff < 0) {
                    for (int i = 0; i > col_diff; i--) { // for each col diff assign a row
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b - 2), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b - 1), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b + 1), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                        image.setPixel((size_t)round(m*(double)((int)col + i) + b + 2), (size_t)((int)col + i), {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
                    }
                }
            }
        }
        last = {row, col};
    }
}

void RoadImage::toPNG(const std::string& file_name) const {
    image.toPNG(file_name);
}
