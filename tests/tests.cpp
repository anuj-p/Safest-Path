#include "../visuals/image.h"
#include "../graphs/RoadGraph.h"

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

void GraphTests() {
    RoadGraph rg(false);
    auto &r1 = rg.insertNode(1,0);
    auto &r2 = rg.insertNode(1,5);
    auto & r3 = rg.insertNode(0,7);
    auto &r4 = rg.insertNode(3,8.5);
    auto & r5 = rg.insertNode(2,10);
    auto & r6 = rg.insertNode(5,10);
    auto &r7 = rg.insertNode(4.5, 5);
    auto &r8 = rg.insertNode(5.3,0);
    auto & r9 = rg.insertNode(10,10);
    auto shortestPath1To9 = rg.BFS(&r1, &r9);
    for (const auto* n : shortestPath1To9) {
        std::cout << "(" << n->pos.x << " " << n->pos.y << ")" << std::endl;
    }
}