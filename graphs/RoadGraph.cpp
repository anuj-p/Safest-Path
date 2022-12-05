
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
    std::map<Point, std::size_t> nodes;
    std::list<Reader::RoadEntry> roadEntries = r.getRoadEntries();
    for (const Reader::RoadEntry& entry : roadEntries) {
        std::size_t prev = UINT32_MAX;
        std::size_t coordListSize = entry.coordinates_list.size();
        double length = entry.length/(coordListSize - 1);
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            std::size_t cur = UINT32_MAX;
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                cur = insertNode(p, entry.name);
                nodes.insert({p, cur});
            } else {
                cur = nodes[p];
            }
            if (prev != UINT32_MAX) {
                insertEdge(prev, cur, length);
            }
            prev = cur;
        }   
    }

    std::list<Reader::TrafficEntry> traffEntries = r.getTrafficEntries();
    for (const Reader::TrafficEntry& entry : traffEntries) {
        std::size_t prev = UINT32_MAX;
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                continue;
            }
            std::size_t cur = nodes[p];
            if (nodes_[cur].name == "") {
                nodes_[cur].name = entry.road_name;
            }
            if (prev != UINT32_MAX) {
                for (std::size_t edge : nodes_[prev].edges) {
                    if (nodes_[edges_[edge].start].id == cur || nodes_[edges_[edge].end].id == cur) {
                        edges_[edge].dailyDriver = static_cast<double>(entry.traffic);
                        break;
                    }
                }
            }
            prev = cur;
        }
    }
    tree_ = KDTree(nodes_);
    std::list<Reader::CrashEntry> crshEntries = r.getCrashEntries();
    for (const Reader::CrashEntry& entry : crshEntries) {
        addCrashNoRecalc(Point(entry.coordinates), static_cast<double>(entry.vehicles));
    }
    recalculateProbAll();
}

std::size_t RoadGraph::insertNode() {
    nodes_.push_back(RoadNode(nextNodeId++));
    return nodes_.size() - 1;
}
std::size_t RoadGraph::insertNode(Point pos) {
    nodes_.push_back(RoadNode(nextNodeId++, pos));
    return nodes_.size() - 1;
}
std::size_t RoadGraph::insertNode(Point pos, std::string name) {
    nodes_.push_back(RoadNode(nextNodeId++, pos, name));
    return nodes_.size() - 1;
}
std::size_t RoadGraph::insertNode(double xPos, double yPos) {
    nodes_.push_back(RoadNode(nextNodeId++, xPos, yPos));
    return nodes_.size() - 1;
}

std::size_t RoadGraph::insertEdge() {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++));
    return edges_.size() - 1;
}
std::size_t RoadGraph::insertEdge(std::size_t node1, std::size_t node2, double length) {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2, length));
    nodes_[node1].edges.push_back(edges_.size() - 1);
    nodes_[node2].edges.push_back(edges_.size() - 1);
    return edges_.size() - 1;
}

bool RoadGraph::containsNode(std::size_t id) const {
    return id < nextNodeId;
}
bool RoadGraph::containsEdge(std::size_t id) const {
    return id < nextEdgeId;
}

std::size_t RoadGraph::addCrash(Point pos, double numCrashes) {
    std::size_t id = tree_.findNearestNeighbor(pos).node;
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return id;
}
std::size_t RoadGraph::addCrash(double xPos, double yPos, double numCrashes) {
    std::size_t id = tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return id;
}

std::size_t RoadGraph::addCrashNoRecalc(Point pos, double numCrashes) {
    std::size_t id = tree_.findNearestNeighbor(pos).node;
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
    }
    return id;
}
std::size_t RoadGraph::addCrashNoRecalc(double xPos, double yPos, double numCrashes) {
    std::size_t id = tree_.findNearestNeighbor(Point(xPos, yPos)).node;
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
    }
    return id;
}

std::size_t RoadGraph::recalculateProb(std::size_t id) {
    RoadNode::RoadEdge& edge = edges_[id];
    if (edge.dailyDriver * 365 <= edge.numCrashes) {
        edge.dailyDriver = edge.numCrashes / 365;
        edge.crashProb = 1;
        return id;
    }
    edge.crashProb = edge.numCrashes / (edge.dailyDriver * 365);
    return id;
}

void RoadGraph::recalculateProbAll() {
    for (RoadNode::RoadEdge& edge : edges_) {
        recalculateProb(edge.id);
    }
}

std::vector<std::size_t> RoadGraph::BFS(Point p1, Point p2){
    return BFS(tree_.findNearestNeighbor(p1).node, tree_.findNearestNeighbor(p2).node);
}

std::vector<std::size_t> RoadGraph::BFS(std::size_t id1, std::size_t id2){   
    if (id1 == id2) {
        return {id1};
    }
    std::queue<std::size_t> q;
    std::set<std::size_t> visited;
    std::map<std::size_t, std::size_t> prev_map;
    bool isConnected = false;
    q.push(id1);
    visited.insert(id1);
    // std::cout << "node1 id: ";
    //     for (size_t t : visited) {
    //         std::cout << t << " ";
    //     }
    // std::cout<< std::endl;
    // std::cout << "got here hello?" << std::endl;
    while (!q.empty()) {

        // std::cout << "visited node ids: ";
        // for (size_t t : visited) {
        //     std::cout << t << " ";
        // }
        // std::cout<< std::endl;

        std::size_t temp = q.front();
        q.pop();
        std::cout << "got here hello?" << std::endl;
        for (std::size_t n : getNeighbors(nodes_[temp].id)) {
            if (visited.find(n) == visited.end()) {
                if (n == id2) {
                    prev_map.insert({id2,temp});
                    visited.insert(n);
                    isConnected = true;
                    break;
                } else {
                    q.push(n);
                    visited.insert(n);
                    prev_map.insert({n, temp});
                } 
            }
        }
    }
    if (isConnected) {
        std::vector<std::size_t> to_return;
        std::size_t temp = id2;
        while (temp != id1) {
            to_return.push_back(temp);
            temp = prev_map.at(temp);
        }
        to_return.push_back(id1);
        std::reverse(to_return.begin(), to_return.end());
        return to_return;
    }
    return {};
}

std::vector<std::size_t> RoadGraph::getNeighbors(std::size_t id){
    std::vector<std::size_t> to_return;
    for (std::size_t edge : nodes_[id].edges) {
        if (nodes_[edges_[edge].start].id == id) {
            to_return.push_back(edges_[id].end);
        } else {
            to_return.push_back(edges_[id].start);
        }
    }
    return to_return;
}

std::pair<std::map<std::size_t, double>, std::map<std::size_t, std::size_t>> RoadGraph::PrimMST(Point p) {
    std::size_t start = tree_.findNearestNeighbor(p).node;
    std::map<std::size_t, double> dist;
    std::map<std::size_t, std::size_t> prev;
    for (RoadNode& n : nodes_) {
        dist.insert({n.id, 2.0});
        prev.insert({n.id, UINT32_MAX});
    }
    dist[start] = 0;

    std::priority_queue<std::pair<double, std::size_t>> q;
    for (RoadNode& n : nodes_) {
        q.push({dist[n.id], n.id});
    }
    std::list<std::size_t> list;

    for (int i = 0; i < nodes_.size(); ++i) {
        std::size_t m = q.top().second;
        list.push_back(m);
        for (std::size_t edge : nodes_[m].edges) {
            std::size_t v = m == edges_[edge].end ? edges_[edge].start : edges_[edge].end;
            if (std::find(list.begin(), list.end(), v) == list.end()) {
                if (1 - (1 - edges_[edge].crashProb)*(1 - dist[m]) < dist[v]) {
                    dist[v] = 1 - (1 - edges_[edge].crashProb)*(1 - dist[m]);
                    prev[v] = m;
                }
            }
        }
    }
    return std::make_pair(dist, prev);
}

std::vector<std::size_t> RoadGraph::DijkstraSSSP(Point p, Point q) {
    std::size_t start = tree_.findNearestNeighbor(p).node;
    std::size_t end = tree_.findNearestNeighbor(q).node;
    std::list<std::size_t> list;
    std::map<std::size_t, std::size_t> prev = PrimMST(p).second;
    for (std::size_t n = end; n != UINT32_MAX; n = prev[n]) {
        list.push_front(n);
    }
    std::vector<std::size_t> vec;
    for (std::size_t n : list) {
        vec.push_back(n);
    }
    return vec;
}

std::map<std::size_t, std::vector<std::size_t>> RoadGraph::BFS(Point p) {
    std::size_t start = tree_.findNearestNeighbor(p).node;
    std::map<std::size_t, std::vector<std::size_t>> graph; 
    graph.insert({start,{}});
    std::queue<std::size_t> q;
    q.push(start);

    while (!q.empty()) {
        std::size_t v = q.front();
        q.pop();
        for (std::size_t w : getNeighbors(v)) {
            if (graph.find(w) == graph.end()) {
                graph[v].push_back(w);
                graph.insert({w,{}});
                q.push(w);
            }
        }
    }
    return graph;
}