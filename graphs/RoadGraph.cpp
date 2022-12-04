
#include "RoadGraph.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <algorithm>

RoadGraph::RoadGraph(bool flag) : nodes_(), edges_(), tree_() {}

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
                for (RoadNode::RoadEdge* edge : prev->edges) {
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

RoadNode::RoadEdge& RoadGraph::insertEdge() {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++));
    return edges_[edges_.size() - 1];
}
RoadNode::RoadEdge& RoadGraph::insertEdge(std::size_t id1, std::size_t id2) {
    RoadNode& node1 = nodes_[id1];
    RoadNode& node2 = nodes_[id2];
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2));
    RoadNode::RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1.edges.push_back(newEdge);
    node2.edges.push_back(newEdge);
    return *newEdge;
}
RoadNode::RoadEdge& RoadGraph::insertEdge(RoadNode& node1, RoadNode& node2) {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2));
    RoadNode::RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1.edges.push_back(newEdge);
    node2.edges.push_back(newEdge);
    return *newEdge;
}
RoadNode::RoadEdge& RoadGraph::insertEdge(RoadNode* node1, RoadNode* node2) {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2));
    RoadNode::RoadEdge* newEdge = &edges_[edges_.size() - 1];
    node1->edges.push_back(newEdge);
    node2->edges.push_back(newEdge);
    return *newEdge;
}
RoadNode::RoadEdge& RoadGraph::insertEdge(RoadNode* node1, RoadNode* node2, double length) {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2, length));
    RoadNode::RoadEdge* newEdge = &edges_[edges_.size() - 1];
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
bool RoadGraph::containsEdge(const RoadNode::RoadEdge& edge) const {
    return edge.id < nextEdgeId;
}

RoadNode& RoadGraph::addCrash(Point pos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(pos).node;
    numCrashes /= node.edges.size();
    for (RoadNode::RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return node;
}
RoadNode& RoadGraph::addCrash(double xPos, double yPos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= node.edges.size();
    for (RoadNode::RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return node;
}

RoadNode& RoadGraph::addCrashNoRecalc(Point pos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(pos).node;
    numCrashes /= node.edges.size();
    for (RoadNode::RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
    }
    return node;
}
RoadNode& RoadGraph::addCrashNoRecalc(double xPos, double yPos, double numCrashes) {
    RoadNode& node = *tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= node.edges.size();
    for (RoadNode::RoadEdge* edge : node.edges) {
        edge->numCrashes += numCrashes;
    }
    return node;
}

RoadNode::RoadEdge& RoadGraph::recalculateProb(RoadNode::RoadEdge& edge) {
    if (edge.dailyDriver * 365 <= edge.numCrashes) {
        edge.dailyDriver = edge.numCrashes / 365;
        edge.crashProb = 1;
        return edge;
    }
    edge.crashProb = edge.numCrashes / (edge.dailyDriver * 365);
    return edge;
}

RoadNode::RoadEdge& RoadGraph::recalculateProb(RoadNode::RoadEdge* edge) {
    return recalculateProb(*edge);
}

bool RoadGraph::recalculateProbAll() {
    for (RoadNode::RoadEdge& edge : edges_) {
        recalculateProb(edge);
    }
    return true;
}

std::vector<RoadNode*> RoadGraph::BFS(Point p1, Point p2){
    return BFS(tree_.findNearestNeighbor(p1).node, tree_.findNearestNeighbor(p2).node);
}

std::vector<RoadNode*> RoadGraph::BFS(RoadNode* node1, RoadNode* node2){
    if (node1->id == node2->id) {
        return {node1};
    }
    std::queue<RoadNode*> q;
    std::set<std::size_t> visited;
    std::map<RoadNode*, RoadNode*> prev_map;
    bool isConnected = false;
    q.push(node1);
    visited.insert(node1->id);
    while (!q.empty()) {
        RoadNode* temp = q.front();
        q.pop();
        for (RoadNode* n : getNeighbors(temp)) {
            if (visited.find(n->id) == visited.end()) {
                if (n->id == node2->id) {
                    prev_map.insert({node2,temp});
                    visited.insert(n->id);
                    isConnected = true;
                    break;
                } else {
                    q.push(n);
                    visited.insert(n->id);
                    prev_map.insert({n, temp});
                } 
            }
        }
    }
    if (isConnected) {
        std::vector<RoadNode*> to_return;
        RoadNode* temp = node2;
        while (temp != node1) {
            to_return.push_back(temp);
            temp = prev_map.at(temp);
        }
        to_return.push_back(node1);
        std::reverse(to_return.begin(), to_return.end());
        return to_return;
    }
    return {};
}

std::vector<RoadNode*> RoadGraph::getNeighbors(RoadNode* node){
    std::vector<RoadNode*> to_return;
    for (const auto* edge : node->edges) {
        if (edge->start->id == node->id) {
            to_return.push_back(edge->end);
        } else {
            to_return.push_back(edge->start);
        }
    }
    return to_return;
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

std::pair<std::map<RoadNode*, double>, std::map<RoadNode*, RoadNode*>> RoadGraph::PrimMST(Point p) {
    RoadNode* start = tree_.findNearestNeighbor(p).node;
    std::map<RoadNode*, double> dist;
    std::map<RoadNode*, RoadNode*> prev;
    for (RoadNode& n : nodes_) {
        dist.insert({&n, 2.0});
        prev.insert({&n, nullptr});
    }
    dist[start] = 0;

    std::priority_queue<std::pair<double, RoadNode*>> q;
    for (RoadNode& n : nodes_) {
        q.push({dist[&n], &n});
    }
    std::list<RoadNode*> list;

    for (int i = 0; i < nodes_.size(); ++i) {
        RoadNode* m = q.top().second;
        list.push_back(m);
        for (RoadNode::RoadEdge* edge : m->edges) {
            RoadNode* v = m == edge->end ? edge->start : edge->end;
            if (std::find(list.begin(), list.end(), v) == list.end()) {
                if (1 - (1 - edge->crashProb)*(1 - dist[m]) < dist[v]) {
                    dist[v] = 1 - (1 - edge->crashProb)*(1 - dist[m]);
                    prev[v] = m;
                }
            }
        }
    }
    return std::make_pair(dist, prev);
}

std::vector<RoadNode*> RoadGraph::DijkstraSSSP(Point p, Point q) {
    RoadNode* start = tree_.findNearestNeighbor(p).node;
    RoadNode* end = tree_.findNearestNeighbor(q).node;
    std::list<RoadNode*> list;
    std::map<RoadNode*, RoadNode*> prev = PrimMST(p).second;
    for (RoadNode* n = end; n != nullptr; n = prev[n]) {
        list.push_front(n);
    }
    std::vector<RoadNode*> vec;
    for (RoadNode* n : list) {
        vec.push_back(n);
    }
    return vec;
}

// void RoadGraph::BFS(Point p) {
//     RoadNode* start = tree_.findNearestNeighbor(p).node;
//     std::map<RoadNode*, bool> visited;
//     std::map<RoadEdge*, unsigned byte> 
//     std::queue<>
// }