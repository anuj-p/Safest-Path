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
        RoadGraph(const std::string& filename);
        RoadGraph(const std::string& filename1, const std::string& filename2, const std::string& filename3);
        RoadNode& insertNode();
        RoadNode& insertNode(Point pos);
        RoadNode& insertNode(double xPos, double yPos);
        RoadNode& insertNode(Point pos, std::string name);
        RoadNode& removeNode(std::size_t id);
        RoadNode::RoadEdge& insertEdge();
        RoadNode::RoadEdge& insertEdge(std::size_t id1, std::size_t id2);
        RoadNode::RoadEdge& insertEdge(const RoadNode& node1, const RoadNode& node2);
        RoadNode::RoadEdge& insertEdge(RoadNode& node1, RoadNode& node2);
        RoadNode::RoadEdge& insertEdge(RoadNode* node1, RoadNode* node2);
        RoadNode::RoadEdge& insertEdge(RoadNode* node1, RoadNode* node2, double length);
        RoadNode::RoadEdge& removeEdge(std::size_t id);
        bool containsNode(std::size_t id) const;
        bool containsNode(RoadNode& node) const;
        bool containsNode(const RoadNode& node) const;
        bool containsEdge(std::size_t id) const;
        bool containsEdge(RoadNode::RoadEdge& edge) const;
        bool containsEdge(const RoadNode::RoadEdge& edge) const;
        RoadNode& addCrash(Point pos, double numCrashes);
        RoadNode& addCrash(double xPos, double yPos, double numCrashes);
        RoadNode& addCrashNoRecalc(Point pos, double numCrashes);
        RoadNode& addCrashNoRecalc(double xPos, double yPos, double numCrashes);
        RoadNode::RoadEdge& recalculateProb(RoadNode::RoadEdge& edge);
        RoadNode::RoadEdge& recalculateProb(RoadNode::RoadEdge* edge);
        std::pair<std::map<RoadNode*, double>, std::map<RoadNode*, RoadNode*>> PrimMST(Point p);
        std::vector<RoadNode*> DijkstraSSSP(Point p, Point q);
        bool recalculateProbAll();
        std::vector<RoadNode*> BFS(Point p1, Point p2);
        std::vector<RoadNode*> BFS(RoadNode* node1, RoadNode* node2);
        std::vector<RoadNode*> getNeighbors(RoadNode* node);
    // private:
        std::vector<RoadNode> nodes_;
        std::vector<RoadNode::RoadEdge> edges_;
        KDTree tree_;
        std::size_t nextNodeId;
        std::size_t nextEdgeId;
        RoadNode& insertNode(std::string pos);
};