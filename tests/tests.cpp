#include "../visuals/image.h"

#include <iostream>
#include <stdexcept>

void ImageTests() {
    std::cout << "-> [Test 1]" << std::endl;
    Image image = Image(50, 50);
    std::tuple<unsigned int, unsigned int, unsigned int> pixel = image.getPixel(49, 49);
    if (std::get<0>(pixel) != 0) throw std::runtime_error("Invalid image initialization.");
    if (std::get<1>(pixel) != 0) throw std::runtime_error("Invalid image initialization.");
    if (std::get<2>(pixel) != 0) throw std::runtime_error("Invalid image initialization.");

    image.setPixel(49, 49, {255, 255, 255});
    pixel = image.getPixel(49, 49);
    if (std::get<0>(pixel) != 255) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<1>(pixel) != 255) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<2>(pixel) != 255) throw std::runtime_error("Invalid pixel manipulation.");

    image.toPPM("tests/test_image_1.ppm");
}

int main() {
    std::cout << "=======================" << std::endl;
    std::cout << "Starting Image tests..." << std::endl;
    ImageTests();
    std::cout << "Completed Image tests." << std::endl;
    std::cout << "=======================" << std::endl;
    return 0;
}