
#include "RoadGraph.h"
#include "utils.h"
#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <unordered_map>

RoadGraph::RoadGraph(const std::list<Reader::RoadEntry>& road_entries, const std::list<Reader::TrafficEntry>& traffic_entries, const std::list<Reader::CrashEntry>& crash_entries) : nodes_(), edges_(), tree_() {
    std::map<Point, std::size_t> nodes;
    for (const Reader::RoadEntry& entry : road_entries) {
        std::size_t prev = static_cast<std::size_t>(-1);
        std::size_t coordListSize = entry.coordinates_list.size();
        double length = entry.length/(coordListSize - 1);
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            std::size_t cur = static_cast<std::size_t>(-1);
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                cur = insertNode(p, entry.name);
                nodes.insert({p, cur});
            } else {
                cur = nodes[p];
            }
            if (prev != static_cast<std::size_t>(-1)) {
                insertEdge(prev, cur, length);
            }
            prev = cur;
        }   
    }

    for (const Reader::TrafficEntry& entry : traffic_entries) {
        std::size_t prev = static_cast<std::size_t>(-1);
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            Point p(pair);
            if (nodes.find(p) == nodes.end()) {
                continue;
            }
            std::size_t cur = nodes[p];
            if (nodes_[cur].name == "") {
                nodes_[cur].name = entry.road_name;
            }
            if (prev != static_cast<std::size_t>(-1)) {
                for (const std::size_t& edge : nodes_[prev].edges) {
                    if (edges_[edge].start == cur || edges_[edge].end == cur) {
                        edges_[edge].dailyDriver = static_cast<double>(entry.traffic);
                        break;
                    }
                }
            }
            prev = cur;
        }
    }
    fillMissingTrafficData();

    std::vector<std::vector<double>> road_entries_vec;
    for (const Reader::RoadEntry& entry : road_entries) {
        for (const std::pair<double, double>& coord : entry.coordinates_list) {
            road_entries_vec.push_back({coord.first, coord.second});
        }
    }
    tree_ = new KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>(2, road_entries_vec, 10);
    tree_->index->buildIndex();

    for (const Reader::CrashEntry& entry : crash_entries) {
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
std::size_t RoadGraph::insertEdge(std::size_t node1, std::size_t node2, double length, double crashProb) {
    edges_.push_back(RoadNode::RoadEdge(nextEdgeId++, node1, node2, crashProb, length));
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
    std::size_t id = findNearestNeighbor({pos.x, pos.y});
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return id;
}
std::size_t RoadGraph::addCrash(double xPos, double yPos, double numCrashes) {
    std::size_t id = findNearestNeighbor({xPos, yPos});
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
        recalculateProb(edge);
    }
    return id;
}

std::size_t RoadGraph::addCrashNoRecalc(Point pos, double numCrashes) {
    std::size_t id = findNearestNeighbor({pos.x, pos.y});
    numCrashes /= nodes_[id].edges.size();
    for (const std::size_t& edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
    }
    return id;
}
std::size_t RoadGraph::addCrashNoRecalc(double xPos, double yPos, double numCrashes) {
    std::size_t id = findNearestNeighbor({xPos, yPos});
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
    for (const RoadNode::RoadEdge& edge : edges_) {
        recalculateProb(edge.id);
    }
}

std::vector<std::size_t> RoadGraph::BFS(Point p1, Point p2){
    return BFS(findNearestNeighbor({p1.x, p1.y}), findNearestNeighbor({p2.x, p2.y}));
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
        // std::cout << "got here hello?" << std::endl;
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
        if (edges_[edge].start == id) {
            to_return.push_back(edges_[edge].end);
        } else {
            to_return.push_back(edges_[edge].start);
        }
    }
    return to_return;
}

std::pair<std::map<std::size_t, double>, std::map<std::size_t, std::size_t>> RoadGraph::PrimMST(Point p) {
    std::size_t start = findNearestNeighbor({p.x, p.y});
    std::map<std::size_t, double> dist;
    std::map<std::size_t, std::size_t> prev;
    for (RoadNode& n : nodes_) {
        dist.insert({n.id, 2.0});
        prev.insert({n.id, static_cast<std::size_t>(-1)});
    }
    dist[start] = 0;

    std::priority_queue<std::pair<double, std::size_t>> q;
    for (RoadNode& n : nodes_) {
        q.push({-dist[n.id], n.id});
    }
    std::unordered_map<std::size_t, bool> map;

    for (int i = 0; i < nodes_.size(); ++i) {
        std::size_t m = q.top().second;
        q.pop();
        map.insert({m, true});
        for (std::size_t edge : nodes_[m].edges) {
            std::size_t v = m == edges_[edge].end ? edges_[edge].start : edges_[edge].end;
            if (map.find(v) == map.end()) {
                if (1 - (1 - edges_[edge].crashProb)*(1 - dist[m]) < dist[v]) {
                    dist[v] = 1 - (1 - edges_[edge].crashProb)*(1 - dist[m]);
                    prev[v] = m;
                    q.push({-dist[v], v});
                }
            }
        }
    }
    for (RoadNode& n : nodes_) {
        if (dist[n.id] == 2.0) {
            dist[n.id] = 1.0;
        }
    }
    return std::make_pair(dist, prev);
}

std::pair<std::unordered_map<std::size_t, double>, std::unordered_map<std::size_t, std::size_t>> RoadGraph::PrimMST(std::size_t start, std::size_t end) {
    std::unordered_map<size_t, double> dist;
    std::unordered_map<size_t, size_t> prev;
    std::priority_queue<std::pair<double, size_t>> queue;
    std::unordered_map<size_t, bool> visited;

    for (const RoadNode& node : nodes_) {
        dist.insert({node.id, 2});
        prev.insert({node.id, static_cast<std::size_t>(-1)});
        queue.push({-2.0, node.id});
        visited.insert({node.id, false});
    }
    dist[start] = 0;
    queue.push({0, start});

    while (!queue.empty() && queue.top().second != end) {
        std::size_t curr = queue.top().second;
        queue.pop();
        visited[curr] = true;
        for (const std::size_t& edge : nodes_[curr].edges) {
            std::size_t neighbor = curr == edges_[edge].end ? edges_[edge].start : edges_[edge].end;
            if (!visited[neighbor]) {
                if (1-(1-dist[curr])*(1-edges_[edge].crashProb) < dist[neighbor]) {
                    dist[neighbor] = 1-(1-dist[curr])*(1-edges_[edge].crashProb);
                    queue.push({-1*dist[neighbor], neighbor});
                    prev[neighbor] = curr;
                }
            }
        }
    }
    for (const RoadNode& node : nodes_) {
        if (dist[node.id] > 1) {
            dist[node.id] = 1;
        }
    }
    return std::make_pair(dist, prev);
}

std::vector<std::size_t> RoadGraph::DijkstraSSSP(std::size_t start, std::size_t end) {
    std::list<std::size_t> list;
    std::unordered_map<std::size_t, std::size_t> prev = PrimMST(start, end).second;
    for (std::size_t n = end; n != static_cast<std::size_t>(-1); n = prev[n]) {
        list.push_front(n);
    }
    std::vector<std::size_t> vec;
    for (const std::size_t& n : list) {
        vec.push_back(n);
    }
    if (vec.empty() || vec[0] != start) {
        return {};
    }
    return vec;
}

std::vector<std::size_t> RoadGraph::DijkstraSSSP(Point p, Point q) {
    std::size_t start = findNearestNeighbor({p.x, p.y});;
    std::size_t end = findNearestNeighbor({q.x, q.y});;
    std::list<std::size_t> list;
    std::map<std::size_t, std::size_t> prev = PrimMST(p).second;
    for (std::size_t n = end; n != static_cast<std::size_t>(-1); n = prev[n]) {
        list.push_front(n);
    }
    std::vector<std::size_t> vec;
    for (std::size_t n : list) {
        vec.push_back(n);
    }
    return vec;
}

std::map<std::size_t, std::vector<std::size_t>> RoadGraph::BFS(Point p) {
    return BFS(findNearestNeighbor({p.x, p.y}));
}

std::map<std::size_t, std::vector<std::size_t>> RoadGraph::BFS(std::size_t start) {
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

void RoadGraph::fillMissingTrafficData() {
    std::size_t start = static_cast<std::size_t>(-1);
    for (std::size_t i = 0; i < nodes_.size(); ++i) {
        for (const std::size_t& edge : nodes_[i].edges) {
            if (edges_[edge].dailyDriver != 0) {
                start = i;
                break;
            }
        }
    }
    if (start != static_cast<std::size_t>(-1)) {
        std::map<std::size_t, std::vector<std::size_t>> graph = BFS(start);
        std::queue<std::size_t> q;
        q.push(start);
        while (!q.empty()) {
            std::size_t v = q.front();
            q.pop();
            for (const std::size_t& edge : nodes_[v].edges) {
                if (edges_[edge].dailyDriver == 0) {
                    double sum = 0.0;
                    std::size_t num = 0;
                    for (const std::size_t& n : nodes_[edges_[edge].start].edges) {
                        if (edges_[n].dailyDriver != 0) {
                            sum += edges_[n].dailyDriver;
                            ++num;
                        }
                    }
                    for (const std::size_t& n : nodes_[edges_[edge].end].edges) {
                        if (edges_[n].dailyDriver != 0) {
                            sum += edges_[n].dailyDriver;
                            ++num;
                        }
                    }
                    edges_[edge].dailyDriver = sum / num;
                }
            }
            for (const std::size_t& n : graph[v]) {
                q.push(n);
            }
        }
    }
}

std::size_t RoadGraph::getEdge(std::size_t start, std::size_t end) {
    for (std::size_t edge : nodes_[start].edges) {
        if (edges_[edge].end == end || edges_[edge].start == end) {
            return edge;
        }
    }
    return static_cast<std::size_t>(-1);
}

size_t RoadGraph::findNearestNeighbor(const std::pair<double, double>& coord) const {
    std::vector<double> coordinates = {coord.first, coord.second};

    size_t num_results = 1;
    nanoflann::KNNResultSet<double> result_set(num_results);
    std::vector<size_t> ret_indexes(num_results);
    std::vector<double> out_dists_sqr(num_results);

    result_set.init(&ret_indexes[0], &out_dists_sqr[0]);

    tree_->index->findNeighbors(result_set, &coordinates[0], nanoflann::SearchParams(10));

    return ret_indexes[0];
}

RoadGraph::~RoadGraph() {
    delete tree_;
}
