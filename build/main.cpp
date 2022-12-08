#include "../visuals/RoadImage.h"
#include "../data/reader.h"
#include "../graphs/RoadGraph.h"
#include "../graphs/nanoflann.hpp"
#include "../graphs/KDTreeVectorOfVectorsAdaptor.h"

#include <vector>
#include <iostream>

int main() {
    std::cout << "Reading roads file..." << std::endl;
    std::list<Reader::RoadEntry> roads = Reader::getRoadEntries();

    // std::cout << "building tree..." << std::endl;
    // std::vector<std::vector<double>> vec;
    // for (const Reader::RoadEntry& road : roads) {
    //     for (const std::pair<double, double>& coords : road.coordinates_list) {
    //         vec.push_back({coords.first, coords.second});
    //     }
    // }

    // typedef KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double> my_kd_tree_t;
    
    // my_kd_tree_t mat_index(2 /*dim*/, vec, 10 /* max leaf */ );
    // mat_index.index->buildIndex();

    // std::list<Reader::CrashEntry> crashes = Reader::getCrashEntries();
    // std::vector<std::vector<double>> vec2;
    // for (const Reader::CrashEntry& crash : crashes) {
    //     vec2.push_back({crash.coordinates.first, crash.coordinates.second});
    // }

    // unsigned int i = 0;
    // for (std::vector<double>& coordinates : vec2) {
    //     size_t num_results = 1;
    //     nanoflann::KNNResultSet<double> resultSet(num_results);
    //     std::vector<size_t> ret_indexes(num_results);
    //     std::vector<double> out_dists_sqr(num_results);

    //     resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);

    //     std::cout << "Searching..." << std::endl;
    //     mat_index.index->findNeighbors(resultSet, &coordinates[0], nanoflann::SearchParams(10));

    //     for (const auto& val : ret_indexes) {
    //         std::cout << "ret_index " << val << std::endl;
    //     }
    //     for (const auto& val : vec[ret_indexes[0]]) {
    //         std::cout << val << std::endl;
    //     }
    //     i++;
    // }

    // std::cout << crashes.size() << std::endl;
    // std::cout << i+1 << std::endl;

    std::cout << "Drawing first image..." << std::endl;
    RoadImage road_image = RoadImage();
    for (const Reader::RoadEntry& road : roads) {
        road_image.addPath(road.coordinates_list, {0, 0, 0});
    }

    std::cout << "Creating road graph..." << std::endl;
    RoadGraph graph = RoadGraph(roads, Reader::getTrafficEntries(), Reader::getCrashEntries());

    std::cout << graph.nodes_.size() << std::endl;
    std::cout << graph.edges_.size() << std::endl;

    std::cout << "Using Dijkstra's..." << std::endl;
    std::vector<size_t> path_idx = graph.DijkstraSSSP(5000000, 1000000);
    std::cout << path_idx.size() << std::endl;

    std::list<std::pair<double, double>> path;
    for (const size_t& idx : path_idx) {
        path.push_back({graph.nodes_[idx].pos.x, graph.nodes_[idx].pos.y});
    }

    road_image.addBoldPath(path, {255, 0, 0});

    std::cout << "Outputting image..." << std::endl;
    road_image.toPNG("new_il.png");
}
