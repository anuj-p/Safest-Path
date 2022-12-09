#include "../visuals/RoadImage.h"
#include "../data/reader.h"
#include "../graphs/RoadGraph.h"
#include "../graphs/nanoflann.hpp"
#include "../graphs/KDTreeVectorOfVectorsAdaptor.h"

#include <vector>
#include <iostream>
#include <queue>

int main() {
    // std::string startPointString = "";
    // std::pair<double, double> startPoint;
    // std::cout << "Input starting point. Use format: (x, y)" << std::endl;
    // std::getline(std::cin, startPointString);
    // size_t start = startPointString.find_first_of('(', 0);
    // size_t mid = startPointString.find_first_of(',', 0);
    // size_t end = startPointString.find_first_of(')', mid);
    // std::string x_coordinate = startPointString.substr(start + 1, mid - start - 1);
    // x_coordinate.erase(x_coordinate.find_last_not_of(' ')+1);
    // x_coordinate.erase(0, x_coordinate.find_first_not_of(' '));

    // std::string y_coordinate = startPointString.substr(mid + 1, end - mid - 1);
    // y_coordinate.erase(y_coordinate.find_last_not_of(' ')+1);
    // y_coordinate.erase(0, y_coordinate.find_first_not_of(' '));

    // double x_start = std::stod(x_coordinate);
    // double y_start = std::stod(y_coordinate);

    // std::string endPointString = "";
    // std::pair<double, double> endPoint;
    // std::cout << "Input end point. Use format: (x, y)" << std::endl;
    // std::getline(std::cin, endPointString);
    // start = endPointString.find_first_of('(', 0);
    // mid = endPointString.find_first_of(',', 0);
    // end = endPointString.find_first_of(')', mid);
    // x_coordinate = endPointString.substr(start + 1, mid - start - 1);
    // x_coordinate.erase(x_coordinate.find_last_not_of(' ')+1);
    // x_coordinate.erase(0, x_coordinate.find_first_not_of(' '));

    // y_coordinate = endPointString.substr(mid + 1, end - mid - 1);
    // y_coordinate.erase(y_coordinate.find_last_not_of(' ')+1);
    // y_coordinate.erase(0, y_coordinate.find_first_not_of(' '));

    // double x_end = std::stod(x_coordinate);
    // double y_end = std::stod(y_coordinate);
    
    // std::cout << x_start << std::endl;
    // std::cout << y_start << std::endl;

    // std::cout << x_end << std::endl;
    // std::cout << y_end << std::endl;


    std::cout << "Reading roads file..." << std::endl;
    std::list<Reader::RoadEntry> roads = Reader::getRoadEntries();    

    std::cout << "Creating road graph..." << std::endl;
    RoadGraph graph = RoadGraph(roads, Reader::getTrafficEntries(), Reader::getCrashEntries());

    std::cout << "Finding nearest neighbor 1..." << std::endl;
    size_t start_node = graph.findNearestNeighbor({-90.0, 40.0});

    // std::cout << "Finding nearest neighbor 2..." << std::endl;
    // size_t end_node = graph.findNearestNeighbor({x_end, y_end});
    
    // std::cout << "Using Dijkstra's..." << std::endl;
    // std::vector<size_t> path_idx = graph.Dijkstra(start_node, end_node);
    
    // RoadImage road_image = RoadImage();
    // for (const Reader::RoadEntry& road : roads) {
    //     road_image.addPath(road.coordinates_list, {0,0,0});
    // }

    // std::list<std::pair<double, double>> path;
    // for (const size_t& idx : path_idx) {
    //     path.push_back(graph.nodes_[idx].pos);
    // }

    // road_image.addBoldPath(path, {255, 0, 0});

    // std::cout << "Outputting image..." << std::endl;
    // road_image.toPNG("new_il.png");

    
}
