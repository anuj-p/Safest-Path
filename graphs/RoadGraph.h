#pragma once

#include "KDTree.h"
#include "../data/reader.h"

#include <vector>
#include <list>
#include <string>
#include <map>

class RoadGraph {
    public:
        RoadGraph();
        RoadGraph(bool flag);
        std::size_t insertNode();
        std::size_t insertNode(Point pos);
        std::size_t insertNode(double xPos, double yPos);
        std::size_t insertNode(Point pos, std::string name);
        std::size_t insertEdge();
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length);
        bool containsNode(std::size_t id) const;
        bool containsEdge(std::size_t id) const;
        std::size_t addCrash(Point pos, double numCrashes);
        std::size_t addCrash(double xPos, double yPos, double numCrashes);
        std::size_t addCrashNoRecalc(Point pos, double numCrashes);
        std::size_t addCrashNoRecalc(double xPos, double yPos, double numCrashes);
        std::size_t recalculateProb(std::size_t id);
        std::pair<std::map<std::size_t, double>, std::map<std::size_t, std::size_t>> PrimMST(Point p);
        std::vector<std::size_t> DijkstraSSSP(Point p, Point q);
        void recalculateProbAll();
        std::vector<std::size_t> BFS(Point p1, Point p2);
        std::vector<std::size_t> BFS(std::size_t node1, std::size_t node2);
        std::map<std::size_t, std::vector<std::size_t>> BFS(Point p);
        std::vector<std::size_t> getNeighbors(std::size_t id);

    // private:
        std::vector<RoadNode> nodes_;
        std::vector<RoadNode::RoadEdge> edges_;
        KDTree tree_;
        std::size_t nextNodeId;
        std::size_t nextEdgeId;
        RoadNode& insertNode(std::string pos);
};