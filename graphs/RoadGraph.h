#pragma once

#include "../data/reader.h"
#include "utils.h"
#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"

#include <vector>
#include <list>
#include <string>
#include <map>
#include <unordered_map>

class RoadGraph {
    public:
        RoadGraph();
        RoadGraph(const std::list<Reader::RoadEntry>& road_entries, const std::list<Reader::TrafficEntry>& traffic_entries, const std::list<Reader::CrashEntry>& crash_entries);
        std::size_t insertNode(std::pair<double, double> pos);
        std::size_t insertNode(std::pair<double, double> pos, std::string name);
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length);
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length, double crashProb);
        std::size_t addCrash(std::pair<double, double> pos, double numCrashes);
        std::size_t addCrashNoRecalc(std::pair<double, double> pos, double numCrashes);
        std::size_t recalculateProb(std::size_t id);
        std::pair<std::vector<double>, std::vector<std::size_t>> PrimMST(std::pair<double, double> p);
        std::pair<std::vector<double>, std::vector<std::size_t>> DijkstraTree(std::size_t start, std::size_t end);
        std::vector<std::size_t> Dijkstra(std::pair<double, double> p, std::pair<double, double> q);
        std::vector<std::size_t> Dijkstra(std::size_t start, std::size_t end);
        void recalculateProbAll();
        std::vector<std::size_t> BFS(std::pair<double, double> p1, std::pair<double, double> p2);
        std::vector<std::size_t> BFS(std::size_t node1, std::size_t node2);
        std::vector<std::vector<std::size_t>> ComponentBFS(std::size_t start);
        std::vector<std::vector<std::size_t>> BFS();
        std::vector<std::vector<std::size_t>> BFS(std::size_t start);
        std::vector<std::vector<std::size_t>> BFS(std::pair<double, double> p);
        std::vector<std::size_t> getNeighbors(std::size_t id);
        size_t findNearestNeighbor(const std::pair<double, double>& coord) const;
        ~RoadGraph();

    // private:
        std::vector<RoadNode> nodes_;
        std::vector<RoadNode::RoadEdge> edges_;
        KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>* tree_;
        void BFSHelper(std::size_t start, std::vector<std::vector<std::size_t>>& bfs, std::vector<bool>& visited);
        void fillMissingTrafficData();
};