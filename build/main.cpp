#include "../visuals/RoadImage.h"
#include "../data/reader.h"
#include "../graphs/RoadGraph.h"
#include "../graphs/nanoflann.hpp"
#include "../graphs/KDTreeVectorOfVectorsAdaptor.h"

#include <vector>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <fstream>

int main(int argc, char *argv[]) {
    std::list<ReaderUtils::RoadEntry> roads;
    RoadGraph graph;
    if (argc == 1) {
        std::cout << "Reading roads file... (~1-3 minutes)" << std::endl;
        roads = std::move(Reader::getRoadEntries("./data/RoadSegment.json"));
        std::cout << "Creating road graph... (~2-4 minutes)" << std::endl;
        graph = std::move(RoadGraph(roads, Reader::getTrafficEntries("./data/Annual_Average_Daily_Traffic_-_2021.geojson"), Reader::getCrashEntries("./data/CRASHES_-_2021.geojson")));
    } else if (argc == 4) {
        std::cout << "Reading roads file... (~1-3 minutes)" << std::endl;
        roads = Reader::getRoadEntries(argv[1]);
        std::cout << "Creating road graph... (~2-4 minutes)" << std::endl;
        graph = RoadGraph(roads, Reader::getTrafficEntries(argv[2]), Reader::getCrashEntries(argv[3]));
    } else {
        throw std::invalid_argument("Invalid number of arguments.");
    }

    std::unordered_map<std::string, std::size_t> nameMap;
    const std::vector<RoadNode>& nodes = graph.getNodes();
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        nameMap.insert({nodes[i].name, i});
    }
    nameMap.erase("");

    std::size_t startNode = static_cast<std::size_t>(-1);
    std::size_t endNode = static_cast<std::size_t>(-1);

    std::cout << "Would you like to enter road names instead of GPS coordinates? Y/N *NOTE: MANY ROADS HAVE THE SAME NAME!" << std::endl;
    std::string inputLine = "";
    std::getline(std::cin, inputLine);
    char inputChar;
    std::stringstream(inputLine) >> inputChar;
    inputChar = std::toupper(inputChar);
    if (inputChar == 'Y') {
        std::cout << "Please enter the starting road name." << std::endl;
        std::string  startRoadName= "";
        std::getline(std::cin, startRoadName);
        if (startRoadName == "") {
            throw std::invalid_argument("Invalid input.");
        }

        std::transform(startRoadName.begin(), startRoadName.end(), startRoadName.begin(), ::toupper);
        auto startIter = nameMap.find(startRoadName);
        if (startIter == nameMap.cend()) {
            throw std::invalid_argument("Invalid input: names not found");
        }

        std::cout << "Please enter the ending road name." << std::endl;
        std::string endRoadName = "";
        std::getline(std::cin, endRoadName);
        if (endRoadName == "") {
            throw std::invalid_argument("Invalid input.");
        }

        std::transform(endRoadName.begin(), endRoadName.end(), endRoadName.begin(), ::toupper);
        auto endIter = nameMap.find(endRoadName);
        if (endIter == nameMap.cend()) {
            throw std::invalid_argument("Invalid input: names not found");
        }

        startNode = startIter->second;
        endNode = endIter->second;
    } else if (inputChar == 'N') {        
        std::cout << "Please input the starting signed GPS coordinates in the format \"(x, y)\"." << std::endl;
        std::string startPointString = "";
        std::getline(std::cin, startPointString);
        size_t start = startPointString.find_first_of('(', 0);
        size_t mid = startPointString.find_first_of(',', 0);
        if (start == static_cast<std::size_t>(-1)|| mid == static_cast<std::size_t>(-1)) {
            throw std::invalid_argument("Invalid input.");
        }
        size_t end = startPointString.find_first_of(')', mid);
        if (end == static_cast<std::size_t>(-1)) {
            throw std::invalid_argument("Invalid input.");
        }
        std::string x_coordinate = startPointString.substr(start + 1, mid - start - 1);
        if (end <= start || mid <= start || end <= mid) {
            throw std::invalid_argument("Invalid input.");
        }
        x_coordinate.erase(x_coordinate.find_last_not_of(' ')+1);
        x_coordinate.erase(0, x_coordinate.find_first_not_of(' '));

        std::string y_coordinate = startPointString.substr(mid + 1, end - mid - 1);
        y_coordinate.erase(y_coordinate.find_last_not_of(' ')+1);
        y_coordinate.erase(0, y_coordinate.find_first_not_of(' '));

        std::pair<double, double> startPoint(std::stod(x_coordinate), std::stod(y_coordinate));
        
        std::cout << "Please input the ending signed GPS coordinates in the format \"(x, y)\"." << std::endl;
        std::string endPointString = "";
        std::getline(std::cin, endPointString);
        start = endPointString.find_first_of('(', 0);
        mid = endPointString.find_first_of(',', 0);
        if (start == static_cast<std::size_t>(-1) || mid == static_cast<std::size_t>(-1)) {
            throw std::invalid_argument("Invalid input.");
        }
        end = endPointString.find_first_of(')', mid);
        if (end == static_cast<std::size_t>(-1)) {
            throw std::invalid_argument("Invalid input.");
        }
        if (end <= start || mid <= start || end <= mid) {
            throw std::invalid_argument("Invalid input.");
        }
        x_coordinate = endPointString.substr(start + 1, mid - start - 1);
        x_coordinate.erase(x_coordinate.find_last_not_of(' ')+1);
        x_coordinate.erase(0, x_coordinate.find_first_not_of(' '));

        y_coordinate = endPointString.substr(mid + 1, end - mid - 1);
        y_coordinate.erase(y_coordinate.find_last_not_of(' ')+1);
        y_coordinate.erase(0, y_coordinate.find_first_not_of(' '));

        std::pair<double, double> endPoint(std::stod(x_coordinate), std::stod(y_coordinate));    

        startNode = graph.findNearestNeighbor(startPoint);
        endNode = graph.findNearestNeighbor(endPoint);
    } else {
        throw std::invalid_argument("Invalid input.");
    }

    std::cout << "Please enter output file in the format \"filename.png\"." << std::endl;
    std::getline(std::cin, inputLine);
    std::ofstream outputFile(inputLine);
    if (outputFile.fail()) {
        throw std::runtime_error("Unable to open " + inputLine + ".");;
    }
    outputFile.close();

    std::cout << "Drawing road network image... (~1-2 minutes)" << std::endl;
    RoadImage road_image = RoadImage(9411, 13000);
    for (const ReaderUtils::RoadEntry& road : roads) {
        road_image.addPath(road.coordinates_list, {0, 0, 0}, -87.5046, -91.5066, 42.5083, 36.9783, 1000);
    }

    // std::cout << graph.nodes_.size() << std::endl;
    // std::cout << graph.edges_.size() << std::endl;

    std::cout << "Using Dijkstra's... (~1-2 minutes)" << std::endl;

    // std::size_t s = graph.findNearestNeighbor({-88.221431, 40.109053});
    // std::size_t e = graph.findNearestNeighbor({-87.635296, 41.878967});
    std::vector<size_t> path_idx = graph.Dijkstra(startNode, endNode);
    // std::cout << path_idx.size() << std::endl;

    std::list<std::pair<double, double>> path;
    std::vector<std::string> directions;
    for (const std::size_t& idx : path_idx) {
        path.push_back(nodes[idx].pos);
        if (directions.empty()) directions.push_back(nodes[idx].name);
        else if (directions.back() != nodes[idx].name) {
            directions.push_back(nodes[idx].name);
        }
    }

    road_image.addBoldPath(path, {255, 0, 0}, -87.5046, -91.5066, 42.5083, 36.9783, 1000);

    std::cout << "Outputting image... (~1 minute)" << std::endl;
    road_image.toPNG(inputLine);
    
    std::cout << "Would you like to view directions? Y/N" << std::endl;
    std::getline(std::cin, inputLine);
    std::stringstream(inputLine) >> inputChar;
    inputChar = std::toupper(inputChar);
    if (inputChar == 'Y') {
        unsigned int i = 1;
        for (const std::string& road : directions) {
            std::cout << "[" << i << "/" << directions.size() << "] Official Road Name: ";
            if (road.empty()) std::cout << "UNNAMED" << std::endl;
            else std::cout << road << std::endl;
            i++;
        }
    } else if (inputChar == 'N') {
    } else {
        throw std::invalid_argument("Invalid input.");
    }

    // std::vector<std::vector<std::size_t>> bfs = graph.ComponentBFS(0);
    // std::queue<std::size_t> queue;
    // queue.push(0);
    // std::size_t count = 0;
    // while (!queue.empty()) {
    //     std::size_t n = queue.front();
    //     queue.pop();
    //     ++count;
    //     for (const std::size_t& i : bfs[n]) {
    //         queue.push(i);
    //     }
    // }
    // std::cout << graph.nodes_.size() << " " << count << std::endl;





    // RoadGraph rg;
    // rg.insertNode({0,0});
    // auto r1 = rg.insertNode({1,0});
    // auto r2 = rg.insertNode({1,5});
    // auto r3 = rg.insertNode({0,7});
    // auto r4 = rg.insertNode({3,8.5});
    // auto r5 = rg.insertNode({2,10});
    // auto r6 = rg.insertNode({5,10});
    // auto r7 = rg.insertNode({4.5, 5});
    // auto r8 = rg.insertNode({5.3,0});
    // auto r9 = rg.insertNode({10,10});

    // rg.insertEdge(r1,r2, 1, 0.2);
    // rg.insertEdge(r2,r3, 1, 0.1);
    // rg.insertEdge(r3,r4, 1, 0.8);
    // rg.insertEdge(r4,r5, 1, 0.1);
    // rg.insertEdge(r5,r6, 1, 0.3);
    // rg.insertEdge(r6,r7, 1, 0.7);
    // rg.insertEdge(r7,r8, 1, 0.4);
    // rg.insertEdge(r8,r9, 1, 0.9);
    // rg.insertEdge(r1,r8, 1, 0.7);
    // rg.insertEdge(r1,r7, 1, 0.3);
    // rg.insertEdge(r2,r7, 1, 0.5);
    // rg.insertEdge(r3,r7, 1, 0.6);
    // rg.insertEdge(r4,r7, 1, 0);
    // rg.insertEdge(r4,r6, 1, 0.5);

    // for (const RoadNode& node : rg.nodes_) {
    //     rg.road_entries_vec_.push_back({node.pos.first, node.pos.second});
    // }
    // rg.tree_ = new KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>(2, rg.road_entries_vec_, 10);
    // rg.tree_->index->buildIndex();

    // RoadImage road_image = RoadImage(400, 400);
    // for (std::size_t i = 0; i < rg.getNodes().size(); ++i) {
    //     for (const auto& neigbor : rg.getNeighbors(i)) {
    //         std::list<std::pair<double, double>> path;
    //         path.push_back(rg.getNodes()[i].pos);
    //         path.push_back(rg.getNodes()[neigbor].pos);
    //         road_image.addPath(path, {0, 0, 0}, 10, 0, 10, 0, 110);
    //     }
    // }

    // std::vector<std::vector<std::size_t>> bfs = rg.BFS(1);
    // for (std::size_t i = 0; i < rg.getNodes().size(); ++i) {
    //     for (const auto& neigbor : bfs[i]) {
    //         std::list<std::pair<double, double>> path;
    //         path.push_back(rg.getNodes()[i].pos);
    //         path.push_back(rg.getNodes()[neigbor].pos);
    //         road_image.addBoldPath(path, {255, 0, 0}, 10, 0, 10, 0, 110);
    //     }
    // }

    // std::cout << "Outputting image..." << std::endl;
    // road_image.toPNG("new_il.png");








    
    // std::cout << x_start << std::endl;
    // std::cout << y_start << std::endl;

    // std::cout << x_end << std::endl;
    // std::cout << y_end << std::endl;

    // std::cout << "Reading roads file..." << std::endl;
    // std::list<Reader::RoadEntry> roads = Reader::getRoadEntries();    

    // std::cout << "Creating road graph..." << std::endl;
    // RoadGraph graph = RoadGraph(roads, Reader::getTrafficEntries(), Reader::getCrashEntries());

    // std::cout << "Finding nearest neighbor 1..." << std::endl;
    // size_t start_node = graph.findNearestNeighbor({-90.0, 40.0});

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
