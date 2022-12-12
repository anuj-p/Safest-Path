#include "RoadImage.h"

#include <list>
#include <cmath>

RoadImage::RoadImage() : image(Image(10000, 13000)), width_(10000), height_(13000) {}

RoadImage::RoadImage(const std::size_t& width, const std::size_t& height) : image(Image(width, height)), width_(width), height_(height) {}

void RoadImage::addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth) {
    std::optional<std::pair<size_t, size_t>> last = std::nullopt;
    for (const std::pair<double, double>& coord : path) {
        std::size_t row = static_cast<std::size_t>(-1);
        std::size_t col = static_cast<std::size_t>(-1);
        if (height_ > width_) {
            row = round((height_ - (coord.second - minLat) * height_ / (maxLat - minLat)) * (height_ - 2 * bordWidth) / height_ + bordWidth);
            col = round((coord.first - minLong) * width_ / (maxLong - minLong) * (width_ - 2.0 * bordWidth * width_ / height_) / width_ + static_cast<double>(bordWidth) * width_ / height_);
        } else {
            row = round((height_ - (coord.second - minLat) * height_ / (maxLat - minLat)) * (height_ - 2.0 * bordWidth * height_ / width_) / height_ + static_cast<double>(bordWidth) * height_ / width_);
            col = round((coord.first - minLong) * width_ / (maxLong - minLong) * (width_ - 2 * bordWidth) / width_ + bordWidth);
        }

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

void RoadImage::addBoldPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth) {
    std::optional<std::pair<size_t, size_t>> last = std::nullopt;
    for (const std::pair<double, double>& coord : path) {
        std::size_t row = static_cast<std::size_t>(-1);
        std::size_t col = static_cast<std::size_t>(-1);
        if (height_ > width_) {
            row = round((height_ - (coord.second - minLat) * height_ / (maxLat - minLat)) * (height_ - 2 * bordWidth) / height_ + bordWidth);
            col = round((coord.first - minLong) * width_ / (maxLong - minLong) * (width_ - 2.0 * bordWidth * width_ / height_) / width_ + static_cast<double>(bordWidth) * width_ / height_);
        } else {
            row = round((height_ - (coord.second - minLat) * height_ / (maxLat - minLat)) * (height_ - 2.0 * bordWidth * height_ / width_) / height_ + static_cast<double>(bordWidth) * height_ / width_);
            col = round((coord.first - minLong) * width_ / (maxLong - minLong) * (width_ - 2 * bordWidth) / width_ + bordWidth);
        }
        
        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                image.setPixel(row + i, col + j, {std::get<0>(color), std::get<1>(color), std::get<2>(color)});
            }
        }
        
        
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
