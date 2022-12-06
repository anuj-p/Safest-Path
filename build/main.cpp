#include "../visuals/RoadImage.h"
#include "../data/reader.h"
#include "../graphs/RoadGraph.h"

#include <vector>
#include <iostream>

int main() {
    std::list<Reader::RoadEntry> roads = Reader::getRoadEntries();

    RoadImage road_image = RoadImage();
    road_image.addPaths(roads, {0, 0, 0});

    RoadGraph graph = RoadGraph();
    std::cout << graph.nodes_.size() << std::endl;
    std::cout << graph.edges_.size() << std::endl;

    std::vector<size_t> path_idx = graph.DijkstraSSSP(1, 500000);

    std::list<std::pair<double, double>> path;
    for (const size_t& idx : path_idx) {
        std::cout << graph.nodes_[idx].pos.x << ", " << graph.nodes_[idx].pos.y << std::endl;
        path.push_back({graph.nodes_[idx].pos.x, graph.nodes_[idx].pos.y});
    }

    road_image.addPath(path, {255, 0, 0});

    road_image.toPNG("new_il.png");
}
