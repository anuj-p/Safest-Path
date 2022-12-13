#pragma once

#include "image.h"
#include "../data/reader.h"

/**
 * RoadImage class
*/
class RoadImage {
    public:
        /**
         * Constructs a RoadImage with width "width" and height "height"
         * 
         * @param width A size_t storing the width of the image
         * @param height A size_t storing the height of the image
        */
        RoadImage(const std::size_t& width, const std::size_t& height);

        /**
         * Draws a 1 pixel wide straight line between every pair of sequential coordinate stored in path
         * 
         * @param path A list of pairs of doubles storing the coordinates in the format {longitude, latitude}
         * @param color A tuple of uint8_t's storing the RGB value of the lines
         * @param maxLong A double storing the max longitude of all coordinates to be drawn on the image
         * @param minLong A double storing the min longitude of all coordinates to be drawn on the image
         * @param maxLat A double storing the max latitude of all coordinates to be drawn on the image
         * @param minLat A double storing the min latitude of all coordinates to be drawn on the image
         * @param bordWidth A size_t storing the border width of the edges of the image for the longer dimension.
         *                  The shorter dimension has a different border width that is calculated so that the
         *                  aspect ratio of the drawable area is equal to the aspect ratio of the image.
        */
        void addPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth);

        /**
         * Draws a 5 pixel wide straight line between every pair of sequential coordinate stored in path
         * 
         * @param path A list of pairs of doubles storing the coordinates in the format {longitude, latitude}
         * @param color A tuple of uint8_t's storing the RGB value of the lines
         * @param maxLong A double storing the max longitude of all coordinates to be drawn on the image
         * @param minLong A double storing the min longitude of all coordinates to be drawn on the image
         * @param maxLat A double storing the max latitude of all coordinates to be drawn on the image
         * @param minLat A double storing the min latitude of all coordinates to be drawn on the image
         * @param bordWidth A size_t storing the border width of the edges of the image for the longer dimension.
         *                  The shorter dimension has a different border width that is calculated so that the
         *                  aspect ratio of the drawable area is equal to the aspect ratio of the image.
        */
        void addBoldPath(std::list<std::pair<double, double>> path, std::tuple<uint8_t, uint8_t, uint8_t> color, double maxLong, double minLong, double maxLat, double minLat, const std::size_t& bordWidth);

        /**
         * Converts the RoadImage into a PNG stored at address "file_name"
         * 
         * @param file_name A string storing the output address of the PNG
        */
        void toPNG(const std::string& file_name) const;
    private:
        /**
         * Internal representation of the road image
         * 
         * "image" stores an Image
         * "width_" stores a size_t of the width of the image
         * "height_" stores a size_t of the height of the image
        */
        Image image_;
        std::size_t width_;
        std::size_t height_;
};
