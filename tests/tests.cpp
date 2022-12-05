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

void GraphTests() {
    RoadGraph rg(false);
    rg.insertNode(0,0);
    auto r1 = rg.insertNode(1,0);
    auto r2 = rg.insertNode(1,5);
    auto r3 = rg.insertNode(0,7);
    auto r4 = rg.insertNode(3,8.5);
    auto r5 = rg.insertNode(2,10);
    auto r6 = rg.insertNode(5,10);
    auto r7 = rg.insertNode(4.5, 5);
    auto r8 = rg.insertNode(5.3,0);
    auto r9 = rg.insertNode(10,10);



    rg.insertEdge(r1,r2, 1, 0.2);
    rg.insertEdge(r2,r3, 1, 0.1);
    rg.insertEdge(r3,r4, 1, 0.8);
    rg.insertEdge(r4,r5, 1, 0.1);
    rg.insertEdge(r5,r6, 1, 0.3);
    rg.insertEdge(r6,r7, 1, 0.7);
    rg.insertEdge(r7,r8, 1, 0.4);
    rg.insertEdge(r8,r9, 1, 0.9);
    rg.insertEdge(r1,r8, 1, 0.7);
    rg.insertEdge(r1,r7, 1, 0.3);
    rg.insertEdge(r2,r7, 1, 0.5);
    rg.insertEdge(r3,r7, 1, 0.6);
    rg.insertEdge(r4,r7, 1, 0);
    rg.insertEdge(r4,r6, 1, 0.5);
    

    for (auto rn : rg.nodes_) {
        std::cout << rn.pos.x << " " << rn.pos.y << " " << rn.id  << " " << &rn << std::endl;
    }

    // std::size_t id4 = rg.tree_.findNearestNeighbor(rg.nodes_[r4].pos).id;
    // std::size_t id7 = rg.tree_.findNearestNeighbor(rg.nodes_[r7].pos).node;

    // std::cout << "id4: " << id4 << "id7: " << id7 << std::endl;

    auto shortestPath4To7 = rg.DijkstraSSSP(r4, r7);
    auto shortestPath1To5 = rg.BFS(r1, r5);
    
    for (auto rn : rg.nodes_) {
        std::cout << rn.pos.x << " " << rn.pos.y << " " << rn.id << std::endl;

    }

    std::cout << "BFS shortest unweighted path test: " << std::endl;
    for (auto n : shortestPath1To5) {
        std::cout << " (" << n << ") ";
    }

    std::cout <<std::endl;
    std::cout << "dijkstra's shortest weighted path test: " << std::endl;
    for (auto n : shortestPath4To7) {
        std::cout << " (" << n << ") ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=======================" << std::endl;
    std::cout << "Starting Image tests..." << std::endl;
    //ImageTests();
    GraphTests();
    std::cout << "Completed Image tests." << std::endl;
    std::cout << "=======================" << std::endl;
    return 0;
}
