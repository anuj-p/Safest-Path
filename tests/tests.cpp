#include "../data/reader.h"
#include "../visuals/image.h"
#include "../graphs/RoadGraph.h"

#include <iostream>

void ImageTests() {
    std::cout << "-> [Test Image Initilization]" << std::endl;
    Image image = Image(50, 50);
    std::tuple<unsigned int, unsigned int, unsigned int> pixel = image.getPixel(49, 49);
    if (std::get<0>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
    if (std::get<1>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
    if (std::get<2>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");

    image.setPixel(49, 49, {0, 0, 0});
    pixel = image.getPixel(49, 49);
    if (std::get<0>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<1>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<2>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");

    image.toPPM("tests/test_image_1.ppm");

    std::cout << "-> [Test Large/Dynamic Image]" << std::endl;

    Image image2 = Image(50, 50);
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; ++j) {
            std::tuple<unsigned int, unsigned int, unsigned int> pixel = image2.getPixel(i, j);
            if (std::get<0>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
            if (std::get<1>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
            if (std::get<2>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
        }
    }

    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; ++j) {
            image2.setPixel(i, j, {i, j, i + j});
        }
    }

    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; ++j) {
            std::tuple<unsigned int, unsigned int, unsigned int> pixel = image2.getPixel(i, j);
            if (std::get<0>(pixel) != i) throw std::runtime_error("Invalid pixel manipulation.");
            if (std::get<1>(pixel) != j) throw std::runtime_error("Invalid pixel manipulation.");
            if (std::get<2>(pixel) != i + j) throw std::runtime_error("Invalid pixel manipulation.");
        }
    }
    image2.toPPM("tests/test_image_2.ppm");

    std::cout << "-> [Test Small Image]" << std::endl;

    Image image3 = Image(1,1);
    pixel = image3.getPixel(0, 0);
    if (std::get<0>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
    if (std::get<1>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");
    if (std::get<2>(pixel) != 255) throw std::runtime_error("Invalid image initialization.");

    image3.setPixel(0, 0, {0, 0, 0});
    pixel = image3.getPixel(0, 0);
    if (std::get<0>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<1>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");
    if (std::get<2>(pixel) != 0) throw std::runtime_error("Invalid pixel manipulation.");

    image3.toPPM("tests/test_image_3.ppm");

}

void ReaderTests() {
    std::cout << "-> [Testing getRoadEntries()]" << std::endl;
    std::list<ReaderUtils::RoadEntry> roadEntryList = Reader::getRoadEntries("./data/RoadSegment.json");
    int roadEntryCounter = 0;
    for (const auto & roadEntryInstance : roadEntryList) {
        if (roadEntryCounter == 0) {
            if (roadEntryInstance.name != "BOAT RAMP RD") throw std::runtime_error("Incorrect road name");
            if (roadEntryInstance.length != 0.001668342829876) throw std::runtime_error("Incorrect road length");

            std::list<std::pair<double, double>> coordinateList = roadEntryInstance.coordinates_list;
            int coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -88.88234799956058 || coordinate.second != 40.15118800017717) throw std::runtime_error("Incorrect coordinates");
                }
                if (coordinateCounter == 5) {
                    if (coordinate.first != -88.88205999956108 || coordinate.second != 40.1520280001759) throw std::runtime_error("Incorrect coordinates");
                    break;
                }
                ++coordinateCounter;
            }
            std::pair<double, double> coordinate = coordinateList.back();
            if (coordinate.first != -88.88150399956191 || coordinate.second != 40.15238300017535) throw std::runtime_error("Incorrect coordinates");
        }

        if (roadEntryCounter == 5) {
            if (roadEntryInstance.name != "9TH PL") throw std::runtime_error("Incorrect road name");
            if (roadEntryInstance.length != 0.00142207451229) throw std::runtime_error("Incorrect road length");

            std::list<std::pair<double, double>> coordinateList = roadEntryInstance.coordinates_list;
            int coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -89.10109400026272 || coordinate.second != 41.33727100041949) throw std::runtime_error("Incorrect coordinates");
                }
                if (coordinateCounter == 1) {
                    if (coordinate.first != -89.10081000026315 || coordinate.second != 41.33729500041943) throw std::runtime_error("Incorrect coordinates");
                    break;
                }
                ++coordinateCounter;
            }
            std::pair<double, double> coordinate = coordinateList.back();
            if (coordinate.first != -89.09967700026493 || coordinate.second != 41.3373910004193) throw std::runtime_error("Incorrect coordinates");

            break;

        }

        ++roadEntryCounter;
    }
    ReaderUtils::RoadEntry roadEntryInstance = roadEntryList.back();
    if (roadEntryInstance.name != "S LOCUST ST") throw std::runtime_error("Incorrect road name");
    if (roadEntryInstance.length != 0.001281224426511) throw std::runtime_error("Incorrect road length");

    std::list<std::pair<double, double>> coordinateList = roadEntryInstance.coordinates_list;
            int coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -88.96017000048147 || coordinate.second != 40.21818900007321) throw std::runtime_error("Incorrect coordinates");
                }
                if (coordinateCounter == 1) {
                    if (coordinate.first != -88.96034000048121 || coordinate.second != 40.21939200007131) throw std::runtime_error("Incorrect coordinates");
                    break;
                }
                ++coordinateCounter;
            }
            std::pair<double, double> coordinate = coordinateList.back();
            if (coordinate.first != -88.9603460004812 || coordinate.second != 40.2194580000712) throw std::runtime_error("Incorrect coordinates");

    
    std::cout << "-> [Testing getTrafficEntries()]" << std::endl;
    std::list<ReaderUtils::TrafficEntry> trafficEntryList = Reader::getTrafficEntries("./data/Annual_Average_Daily_Traffic_-_2021.geojson");
    int trafficEntryCounter = 0;
    for (const auto & trafficEntryInstance : trafficEntryList) {
        if (trafficEntryCounter == 0) {
            if (trafficEntryInstance.road_name != "WATSON RD") throw std::runtime_error("Incorrect road name");
            if (trafficEntryInstance.traffic != 50) throw std::runtime_error("Incorrect traffic value");

            std::list<std::pair<double, double>> coordinateList = trafficEntryInstance.coordinates_list;
            int coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -89.343826055063118 || coordinate.second != 37.071956950407937) throw std::runtime_error("Incorrect coordinates");
                }
                if (coordinateCounter == 5) {
                    if (coordinate.first != -89.325545990297528 || coordinate.second != 37.071856961531125) throw std::runtime_error("Incorrect coordinates");
                    break;
                }
                ++coordinateCounter;
            }
            std::pair<double, double> coordinate = coordinateList.back();
            if (coordinate.first != -89.317471052886276 || coordinate.second != 37.071767428315233) throw std::runtime_error("Incorrect coordinates");
        }
        if (trafficEntryCounter == 9) {
            if (trafficEntryInstance.road_name != "SHILOH BAPTIST CH RD") throw std::runtime_error("Incorrect road name");
            if (trafficEntryInstance.traffic != 125) throw std::runtime_error("Incorrect traffic value");

            std::list<std::pair<double, double>> coordinateList = trafficEntryInstance.coordinates_list;
            int coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -89.187335778515873 || coordinate.second != 37.031076895604812) throw std::runtime_error("Incorrect coordinates");
                }
                if (coordinateCounter == 1) {
                    if (coordinate.first != -89.18648747937047 || coordinate.second != 37.031069365489529) throw std::runtime_error("Incorrect coordinates");
                    break;
                }

            ++coordinateCounter;
        }
        std::pair<double, double> coordinate = coordinateList.back();
        if (coordinate.first != -89.186312146639821 || coordinate.second != 37.031067808802725) throw std::runtime_error("Incorrect coordinates");
        break;
    }
        ++trafficEntryCounter;
    }
    ReaderUtils::TrafficEntry trafficEntryInstance = trafficEntryList.back();
    if (trafficEntryInstance.road_name != "HIGH") throw std::runtime_error("Incorrect road name");
    if (trafficEntryInstance.traffic != 250) throw std::runtime_error("Incorrect traffic value");

    coordinateList = trafficEntryInstance.coordinates_list;
    coordinateCounter = 0;
            for (const auto & coordinate : coordinateList) {
                if (coordinateCounter == 0) {
                    if (coordinate.first != -90.433776623703608 || coordinate.second != 42.413047779907558) throw std::runtime_error("Incorrect coordinates");

                }
                if (coordinateCounter == 1) {
                    if (coordinate.first != -90.433497436166419 || coordinate.second != 42.413527641970205) throw std::runtime_error("Incorrect coordinates");
                    break;
                }
                ++coordinateCounter;
            }
    coordinate = coordinateList.back();
    if (coordinate.first != -90.433476525862616 || coordinate.second != 42.413579887860024) throw std::runtime_error("Incorrect coordinates");

    
    std::cout << "-> [Testing getCrashEntries()]" << std::endl;
    std::list<ReaderUtils::CrashEntry> crashEntryList = Reader::getCrashEntries("./data/CRASHES_-_2021.geojson");
    int crashEntryCounter = 0;
    for (const auto & crashEntryInstance : crashEntryList) {
        if (crashEntryCounter == 0) {
            if (crashEntryInstance.vehicles != 2) throw std::runtime_error("Incorrect vehicles");
            if (crashEntryInstance.coordinates.second != 41.7581 || crashEntryInstance.coordinates.first != -89.4134) throw std::runtime_error("Incorrect coordinates");
        }
        if (crashEntryCounter == 6) {
            if (crashEntryInstance.vehicles != 3) throw std::runtime_error("Incorrect vehicles");
            if (crashEntryInstance.coordinates.second != 41.80927379 || crashEntryInstance.coordinates.first != -87.61662205) throw std::runtime_error("Incorrect coordinates");
            break;
        }
        ++crashEntryCounter;
    }

    ReaderUtils::CrashEntry crashEntryInstance = crashEntryList.back();
    if (crashEntryInstance.vehicles != 2) throw std::runtime_error("Incorrect vehicles");
    if (crashEntryInstance.coordinates.second != 41.75000067 || crashEntryInstance.coordinates.first != -87.70225383) throw std::runtime_error("Incorrect coordinates");

}

void GraphTests() {
    std::cout << "-> [Testing RoadGraph Construction]" << std::endl;

    RoadGraph rg;
    rg.insertNode({0,0});
    std::size_t r1 = rg.insertNode({1,0});
    std::size_t r2 = rg.insertNode({1,5});
    std::size_t r3 = rg.insertNode({0,7});
    std::size_t r4 = rg.insertNode({3,8.5});
    std::size_t r5 = rg.insertNode({2,10});
    std::size_t r6 = rg.insertNode({5,10});
    std::size_t r7 = rg.insertNode({4.5, 5});
    std::size_t r8 = rg.insertNode({5.3,0});
    std::size_t r9 = rg.insertNode({10,10});

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
    rg.insertEdge(r6,r9, 1, 0.1);

    if (rg.getNodes().size() != 10) throw std::runtime_error("insertNode did not insert all nodes into graph");
    if (rg.getEdges().size() != 15) throw std::runtime_error("insertEdge did not insert all edges into graph");
    if (!(rg.getNodes().at(0).pos.first == 0 && rg.getNodes().at(0).pos.second == 0)) throw std::runtime_error("graph has incorrect node position value");
    if (!(rg.getNodes().at(3).pos.first == 0 && rg.getNodes().at(3).pos.second == 7)) throw std::runtime_error("graph has incorrect node position value");
    if (!(rg.getNodes().at(7).pos.first == 4.5 && rg.getNodes().at(7).pos.second == 5)) throw std::runtime_error("graph has incorrect node position value");
    if (rg.getEdges().at(9).start != 1) throw std::runtime_error("graph has incorrect edge start/end node value");
    if (rg.getEdges().at(13).end != 6) throw std::runtime_error("graph has incorrect edge start/end node value");
    if (rg.getEdges().at(0).start != 1) throw std::runtime_error("graph has incorrect edge start/end node value");
    if (rg.getEdges().at(0).end != 2) throw std::runtime_error("graph has incorrect edge start/end node value");

    std::cout << "-> [Testing Dijkstra()]" << std::endl;

    std::vector<std::size_t> shortestPath4To7 = rg.Dijkstra(r4, r7);
    if (shortestPath4To7.size() != 2) throw std::runtime_error("Dijkstra path incorrect length");
    if (shortestPath4To7[0] != 4) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath4To7[1] != 7) throw std::runtime_error("Dijkstra path incorrect nodes");

    std::vector<std::size_t> shortestPath1To9 = rg.Dijkstra(r1, r9);
    if (shortestPath1To9.size() != 6) throw std::runtime_error("Dijkstra path incorrect length");
    if (shortestPath1To9[0] != 1) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath1To9[1] != 7) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath1To9[2] != 4) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath1To9[3] != 5) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath1To9[4] != 6) throw std::runtime_error("Dijkstra path incorrect nodes");
    if (shortestPath1To9[5] != 9) throw std::runtime_error("Dijkstra path incorrect nodes");

    std::cout << "-> [Testing BFSPath()]" << std::endl;

    std::vector<std::size_t> shortestPath1To5 = rg.BFSPath(r1, r5);
    if (shortestPath1To5.size() != 4) throw std::runtime_error("BFS path incorrect length");
    if (shortestPath1To5[0] != 1) throw std::runtime_error("BFS path incorrect nodes");
    if (shortestPath1To5[1] != 7) throw std::runtime_error("BFS path incorrect nodes");
    if (shortestPath1To5[2] != 6) throw std::runtime_error("BFS path incorrect nodes");
    if (shortestPath1To5[3] != 5) throw std::runtime_error("BFS path incorrect nodes");

    std::cout << "-> [Testing BFS()]" << std::endl;

    std::vector<std::vector<std::size_t>> graphFrom1 = rg.BFS(r1);
    std::vector<std::size_t> row1Nodes({2, 8, 7});
    std::vector<std::size_t> row2Nodes({3});
    std::vector<std::size_t> row6Nodes({5});
    std::vector<std::size_t> row7Nodes({6,4});
    std::vector<std::size_t> row8Nodes({9});
    if (graphFrom1.size() != 10) throw std::runtime_error("BFS Graph generator doesn't cover full graph");
    if (!graphFrom1[0].empty()) throw std::runtime_error("BFS Graph generator has incorrect connections");
    if (!(graphFrom1[1] == row1Nodes)) throw std::runtime_error("BFS Graph generator has incorrect connections 1");
    if (!(graphFrom1[2] == row2Nodes)) throw std::runtime_error("BFS Graph generator has incorrect connections 2");
    if (!graphFrom1[3].empty()) throw std::runtime_error("BFS Graph generator has incorrect connections");
    if (!graphFrom1[4].empty()) throw std::runtime_error("BFS Graph generator has incorrect connections");
    if (!graphFrom1[5].empty()) throw std::runtime_error("BFS Graph generator has incorrect connections");
    if (!(graphFrom1[6] == row6Nodes)) throw std::runtime_error("BFS Graph generator has incorrect connections 3");
    if (!(graphFrom1[7] == row7Nodes)) throw std::runtime_error("BFS Graph generator has incorrect connections 4");
    if (!(graphFrom1[8] == row8Nodes)) throw std::runtime_error("BFS Graph generator has incorrect connections 5");
    if (!graphFrom1[9].empty()) throw std::runtime_error("BFS Graph generator has incorrect connections");

    std::cout << "-> [Testing findNearestNeighbor()]" << std::endl;

    rg.buildTree();
    std::size_t id4 = rg.findNearestNeighbor(rg.getNodes().at(r4).pos);
    std::size_t id7 = rg.findNearestNeighbor(rg.getNodes().at(r7).pos);
    std::size_t id2 = rg.findNearestNeighbor({0, 5});
    std::size_t id9 = rg.findNearestNeighbor({100, 100});
    if (id4 != 4) throw std::runtime_error("wrong nearest node");
    if (id7 != 7) throw std::runtime_error("wrong nearest node");
    if (id2 != 2) throw std::runtime_error("wrong nearest node");
    if (id9 != 9) throw std::runtime_error("wrong nearest node");
}

int main() {
    std::cout << "=======================" << std::endl;
    std::cout << "Starting Reader tests..." << std::endl;
    ReaderTests();
    std::cout << "=======================" << std::endl;
    std::cout << "Completed Reader tests..." << std::endl;

    std::cout << "=======================" << std::endl;
    std::cout << "Starting Image tests..." << std::endl;
    ImageTests();
    std::cout << "Completed Image tests." << std::endl;
    std::cout << "=======================" << std::endl;

    std::cout << "=======================" << std::endl;
    std::cout << "Starting Graph tests..." << std::endl;
    GraphTests();
    std::cout << "Completed Graph tests." << std::endl;
    std::cout << "=======================" << std::endl;

    return 0;
}
