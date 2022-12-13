#pragma once

/**
 * Image class
*/
class Image {
    public:
        /**
         * Constructs an Image with a width of "width" and a height of "height"
         * 
         * @param width A size_t storing the width of the image
         * @param height A size_t storing the height of the image
        */
        Image(const size_t& width, const size_t& height);

        /**
         * Returns the pixel at (x,y) = (col,row) in RGB format
         * 
         * @param row A size_t storing the y-value of the pixel
         * @param col A size_t storing the x-value of the pixel
         * @return A tuple storing the RGB value of the pixel
        */
        std::tuple<uint8_t, uint8_t, uint8_t> getPixel(const size_t& row, const size_t& col) const;

        /**
         * Sets the RGB value of the pixel at (x,y) = (col,row)
         * 
         * @param row A size_t storing the y-value of the pixel
         * @param col A size_t storing the x-value of the pixel
         * @param val A tuple of uint8_t's storing the future RGB value of the pixel
        */
        void setPixel(const size_t& row, const size_t& col, const std::tuple<uint8_t, uint8_t, uint8_t>& val);

        /**
         * Converts the Image into a PPM stored at address "file_name"
         * 
         * @param file_name A string storing the output address of the PPM
        */
        void toPPM(const std::string& file_name) const;

        /**
         * Converts the Image into a PNG stored at address "file_name"
         * 
         * @param file_name A string storing the output address of the PNG
        */
        void toPNG(const std::string& file_name) const;

        /**
         * Image Destructor
        */
        ~Image();
    private:
        /**
         * Internal representation of the image
         * 
         * "*image_" stores a pointer to the array of tuples that represent the image in RGB format
         * "width_" stores a size_t of the width of the image
         * "height_" stores a size_t of the height of the image
        */
        std::tuple<uint8_t, uint8_t, uint8_t>* image_;
        size_t width_;
        size_t height_;
};
