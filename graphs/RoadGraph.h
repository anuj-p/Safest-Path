#pragma once

#include "KDTree.h"
#include "reader.h"
#include "RoadEdge.h"

#include <vector>
#include <list>
#include <string>

class RoadGraph {
    public:
        RoadGraph();
        RoadGraph(bool flag);
        RoadGraph(const std::string& filename);
        RoadGraph(const std::string& filename1, const std::string& filename2, const std::string& filename3);
        RoadNode& insertNode();
        RoadNode& insertNode(Point pos);
        RoadNode& insertNode(double xPos, double yPos);
        RoadNode& RoadGraph::insertNode(Point pos, std::string name);
        RoadNode& removeNode(std::size_t id);
        RoadEdge& insertEdge();
        RoadEdge& insertEdge(std::size_t id1, std::size_t id2);
        RoadEdge& insertEdge(const RoadNode& node1, const RoadNode& node2);
        RoadEdge& insertEdge(RoadNode& node1, RoadNode& node2);
        RoadEdge& insertEdge(RoadNode* node1, RoadNode* node2);
        RoadEdge& RoadGraph::insertEdge(RoadNode* node1, RoadNode* node2, double length);
        RoadEdge& removeEdge(std::size_t id);
        bool containsNode(std::size_t id) const;
        bool containsNode(RoadNode& node) const;
        bool containsNode(const RoadNode& node) const;
        bool containsEdge(std::size_t id) const;
        bool containsEdge(RoadEdge& edge) const;
        bool containsEdge(const RoadEdge& edge) const;
        RoadNode& addCrash(Point pos, double numCrashes);
        RoadNode& addCrash(double xPos, double yPos, double numCrashes);
        RoadNode& addCrashNoRecalc(Point pos, double numCrashes);
        RoadNode& addCrashNoRecalc(double xPos, double yPos, double numCrashes);
        RoadEdge& recalculateProb(RoadEdge& edge);
        RoadEdge& recalculateProb(RoadEdge* edge);
        std::pair<std::map<RoadNode*, double>, std::map<RoadNode*, RoadNode*>> PrimMST(Point p);
        std::vector<RoadNode*> DijkstraSSSP(Point p, Point q);
        void RoadGraph::BFS();
        bool recalculateProbAll();
    private:
        std::vector<RoadNode> nodes_;
        std::vector<RoadEdge> edges_;
        KDTree tree_;
        std::size_t nextNodeId;
        std::size_t nextEdgeId;
        RoadNode& insertNode(std::string pos);
};