
#include "RoadGraph.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <map>

RoadGraph::RoadGraph() : nodes_(), edges_(), tree_() {
    Reader r;
    std::map<Point, RoadNode*> nodes;
    std::list<Reader::RoadEntry> roadEntries = r.getRoadEntries();
    for (const Reader::RoadEntry& entry : roadEntries) {
        RoadNode* prev = nullptr;
        std::size_t coordListSize = entry.coordinates_list.size();
        double length = entry.length/(coordListSize - 1);
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            RoadNode* cur = nullptr;
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                cur = &insertNode(p, entry.name);
                nodes.insert({p, cur});
            } else {
                cur = nodes[p];
            }
            if (prev) {
                insertEdge(prev, cur, length);
            }
            prev = cur;
        }   
    }
    tree_ = KDTree(nodes_);
    std::list<Reader::TrafficEntry> traffEntries = r.getTrafficEntries();
    for (const Reader::TrafficEntry& entry : traffEntries) {
        RoadNode* prev = nullptr;
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                continue;
            }
            RoadNode* cur = nodes[p];
            if (cur->name == "") {
                cur->name = entry.road_name;
            }
            if (prev) {
                for (RoadEdge* edge : prev->edges) {
                    if (edge->start->id == cur->id || edge->end->id == cur->id) {
                        edge->dailyDriver = static_cast<double>(entry.traffic);
                        break;
                    }
                }
            }
            prev = cur;
        }
    }
    std::list<Reader::CrashEntry> crshEntries = r.getCrashEntries();
    for (const Reader::CrashEntry& entry : crshEntries) {
        addCrashNoRecalc(Point(entry.coordinates), static_cast<double>(entry.vehicles));
    }
    recalculateProbAll();
}

RoadNode& RoadGraph::insertNode() {
    nodes_.push_back(RoadNode(nextNodeId++));
    return nodes_[nodes_.size() - 1];
}
RoadNode& RoadGraph::insertNode(Point pos) {
    nodes_.push_back(RoadNode(nextNodeId++, pos));
    return nodes_[nodes_.size() - 1];
}
RoadNode& RoadGraph::insertNode(Point pos, std::string name) {
    nodes_.push_back(RoadNode(nextNodeId++, pos, name));
    return nodes_[nodes_.size() - 1];
}
RoadNode& RoadGraph::insertNode(double xPos, double yPos) {
    nodes_.push_back(RoadNode(nextNodeId++, xPos, yPos));
    return nodes_[nodes_.size() - 1];
}
RoadNode& RoadGraph::insertNode(std::string pos) {
    nodes_.push_back(RoadNode(nextNodeId++, pos));
    return nodes_[nodes_.size() - 1];
}

// RoadNode& RoadGraph::removeNode(std::size_t id);

RoadEdge& RoadGraph::insertEdge() {
    edges_.push_back(RoadEdge(nextEdgeId++));
    return edges_[edges_.size() - 1];
}
RoadEdge& RoadGraph::insertEdge(std::size_t id1, std::size_t id2) {
    RoadNode& node1 = nodes_[id1];
    RoadNode& node2 = nodes_[id2];
    edges_.push_back(RoadEdge(nextEdgeId++, node1, node2));
    RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1.edges.push_back(newEdge);
    node2.edges.push_back(newEdge);
    return *newEdge;
}
RoadEdge& RoadGraph::insertEdge(RoadNode& node1, RoadNode& node2) {
    edges_.push_back(RoadEdge(nextEdgeId++, node1, node2));
    RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1.edges.push_back(newEdge);
    node2.edges.push_back(newEdge);
    return *newEdge;
}
RoadEdge& RoadGraph::insertEdge(RoadNode* node1, RoadNode* node2) {
    edges_.push_back(RoadEdge(nextEdgeId++, node1, node2));
    RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1->edges.push_back(newEdge);
    node2->edges.push_back(newEdge);
    return *newEdge;
}
RoadEdge& RoadGraph::insertEdge(RoadNode* node1, RoadNode* node2, double length) {
    edges_.push_back(RoadEdge(nextEdgeId++, node1, node2, length));
    RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1->edges.push_back(newEdge);
    node2->edges.push_back(newEdge);
    return *newEdge;
}

// RoadEdge& RoadGraph::removeEdge(std::size_t id);

bool RoadGraph::containsNode(std::size_t id) const {
    return id < nextNodeId;
}
bool RoadGraph::containsNode(const RoadNode& node) const {
    return node.id < nextNodeId;
}
bool RoadGraph::containsEdge(std::size_t id) const {
    return id < nextEdgeId;
}
bool RoadGraph::containsEdge(const RoadEdge& edge) const {
    return edge.id < nextEdgeId;
}

RoadNode& RoadGraph::addCrash(Point pos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(pos).node;
    numCrashes /= node.edges.size();
    for (RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return node;
}
RoadNode& RoadGraph::addCrash(double xPos, double yPos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= node.edges.size();
    for (RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return node;
}

RoadNode& RoadGraph::addCrashNoRecalc(Point pos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(pos).node;
    numCrashes /= node.edges.size();
    for (RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
    }
    return node;
}
RoadNode& RoadGraph::addCrashNoRecalc(double xPos, double yPos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= node.edges.size();
    for (RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
    }
    return node;
}

RoadEdge& RoadGraph::recalculateProb(RoadEdge& edge) {
    if (edge.dailyDriver * 365 <= edge.numCrashes) {
        edge.dailyDriver = edge.numCrashes / 365;
        edge.crashProb = 1;
        return edge;
    }
    edge.crashProb = edge.numCrashes / (edge.dailyDriver * 365);
    return edge;
}

RoadEdge& RoadGraph::recalculateProb(RoadEdge* edge) {
    return recalculateProb(*edge);
}

bool RoadGraph::recalculateProbAll() {
    for (RoadEdge& edge : edges_) {
        recalculateProb(edge);
    }
    return true;
}

// RoadGraph* RoadGraph::PrimMST(Point p) {
//     return PrimMST(p.x, p.y);
// }

// RoadGraph* RoadGraph::PrimMST(double x, double y) {
//     KDPoint p = tree_.findNearestNeighbor(x, y);
// }

// RoadGraph* RoadGraph::PrimMST(const RoadNode& node) {
//     return PrimMST(node.)
// }

