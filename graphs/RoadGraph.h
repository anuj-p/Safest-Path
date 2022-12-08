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
        RoadGraph(const std::list<Reader::RoadEntry>& road_entries, const std::list<Reader::TrafficEntry>& traffic_entries, const std::list<Reader::CrashEntry>& crash_entries);
        std::size_t insertNode();
        std::size_t insertNode(Point pos);
        std::size_t insertNode(double xPos, double yPos);
        std::size_t insertNode(Point pos, std::string name);
        std::size_t insertEdge();
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length);
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length, double crashProb);
        bool containsNode(std::size_t id) const;
        bool containsEdge(std::size_t id) const;
        std::size_t addCrash(Point pos, double numCrashes);
        std::size_t addCrash(double xPos, double yPos, double numCrashes);
        std::size_t addCrashNoRecalc(Point pos, double numCrashes);
        std::size_t addCrashNoRecalc(double xPos, double yPos, double numCrashes);
        std::size_t recalculateProb(std::size_t id);
        std::pair<std::map<std::size_t, double>, std::map<std::size_t, std::size_t>> PrimMST(Point p);
        std::pair<std::unordered_map<std::size_t, double>, std::unordered_map<std::size_t, std::size_t>> PrimMST(std::size_t start, std::size_t end);
        std::vector<std::size_t> DijkstraSSSP(Point p, Point q);
        std::vector<std::size_t> DijkstraSSSP(std::size_t start, std::size_t end);
        void recalculateProbAll();
        std::vector<std::size_t> BFS(Point p1, Point p2);
        std::vector<std::size_t> BFS(std::size_t node1, std::size_t node2);
        std::map<std::size_t, std::vector<std::size_t>> BFS(Point p);
        std::map<std::size_t, std::vector<std::size_t>> BFS(std::size_t start);
        std::vector<std::size_t> getNeighbors(std::size_t id);
        std::size_t getEdge(std::size_t start, std::size_t end);
        size_t findNearestNeighbor(const std::pair<double, double>& coord) const;
        ~RoadGraph();

    // private:
        std::vector<RoadNode> nodes_;
        std::vector<RoadNode::RoadEdge> edges_;
        KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>* tree_;
        std::size_t nextNodeId;
        std::size_t nextEdgeId;
        void fillMissingTrafficData();
};