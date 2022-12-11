#include "RoadGraph.h"

#include <map>
#include <queue>
#include <set>
#include <cmath>

RoadGraph::RoadGraph() : nodes_(), edges_(), road_entries_vec_(), tree_(nullptr) {}

RoadGraph::RoadGraph(const std::list<Reader::RoadEntry>& road_entries, const std::list<Reader::TrafficEntry>& traffic_entries, const std::list<Reader::CrashEntry>& crash_entries) : nodes_(), edges_(), road_entries_vec_(), tree_(nullptr) {
    std::map<std::pair<double, double>, std::size_t> nodes;
    for (const Reader::RoadEntry& entry : road_entries) {
        std::size_t prev = static_cast<std::size_t>(-1);
        std::pair<double, double> prev_pos;
        std::size_t coordListSize = entry.coordinates_list.size();
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            std::size_t cur = static_cast<std::size_t>(-1);
            if (nodes.find(pair) == nodes.end()) {
                cur = insertNode(pair, entry.name);
                nodes.insert({pair, cur});
            } else {
                cur = nodes[pair];
            }
            std::pair<double, double> cur_pos = nodes_[cur].pos;
            if (prev != static_cast<std::size_t>(-1)) {
                double dist = sqrt((cur_pos.first - prev_pos.first) * (cur_pos.first - prev_pos.first) + (cur_pos.second - prev_pos.second) * (cur_pos.second - prev_pos.second));
                insertEdge(prev, cur, dist);
            }
            prev = cur;
            prev_pos = cur_pos;
        }   
    }

    for (const RoadNode& node : nodes_) {
        road_entries_vec_.push_back({node.pos.first, node.pos.second});
    }
    tree_ = new KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>(2, road_entries_vec_, 10);
    tree_->index->buildIndex();

    for (const Reader::TrafficEntry& entry : traffic_entries) {
        std::size_t prev = static_cast<std::size_t>(-1);
        for (const std::pair<double, double>& pair : entry.coordinates_list) {
            std::size_t cur = static_cast<std::size_t>(-1);
            if (nodes.find(pair) != nodes.end()) {
                cur = nodes[pair];
            } else {
                cur = findNearestNeighbor(pair);
            }
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



    for (const Reader::CrashEntry& entry : crash_entries) {
        addCrashNoRecalc(entry.coordinates, static_cast<double>(entry.vehicles));
    }
    recalculateProbAll();
}

std::size_t RoadGraph::insertNode(std::pair<double, double> pos) {
    nodes_.push_back(RoadNode(nodes_.size(), pos));
    return nodes_.size() - 1;
}
std::size_t RoadGraph::insertNode(std::pair<double, double> pos, std::string name) {
    nodes_.push_back(RoadNode(nodes_.size(), pos, name));
    return nodes_.size() - 1;
}

std::size_t RoadGraph::insertEdge(std::size_t node1, std::size_t node2, double length) {
    edges_.push_back(RoadNode::RoadEdge(edges_.size(), node1, node2, length));
    nodes_[node1].edges.push_back(edges_.size() - 1);
    nodes_[node2].edges.push_back(edges_.size() - 1);
    return edges_.size() - 1;
}
std::size_t RoadGraph::insertEdge(std::size_t node1, std::size_t node2, double length, double crashProb) {
    edges_.push_back(RoadNode::RoadEdge(edges_.size(), node1, node2, crashProb, length));
    nodes_[node1].edges.push_back(edges_.size() - 1);
    nodes_[node2].edges.push_back(edges_.size() - 1);
    return edges_.size() - 1;
}

std::size_t RoadGraph::addCrash(std::pair<double, double> pos, double numCrashes) {
    std::size_t id = findNearestNeighbor(pos);
    numCrashes /= nodes_[id].edges.size();
    for (std::size_t edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
        recalculateProb(edge, 0.00000001, 0.00000001, 0);
    }
    return id;
}

std::size_t RoadGraph::addCrashNoRecalc(std::pair<double, double> pos, double numCrashes) {
    std::size_t id = findNearestNeighbor(pos);
    numCrashes /= nodes_[id].edges.size();
    for (const std::size_t& edge : nodes_[id].edges) {
        edges_[edge].numCrashes += numCrashes;
    }
    return id;
}

std::size_t RoadGraph::recalculateProb(std::size_t id, double constantFactor, double lengthFactor, double driverFactor) {
    RoadNode::RoadEdge& edge = edges_[id];
    if (edge.dailyDriver == 0 && edge.numCrashes == 0) {
        edge.crashProb = 0;
    } else if (edge.dailyDriver * 365 <= edge.numCrashes) {
        edge.dailyDriver = edge.numCrashes / 365;
        edge.crashProb = 1;
    } else {
        edge.crashProb = edge.numCrashes / (edge.dailyDriver * 365);
    }
    edge.crashProb += constantFactor + lengthFactor * edge.length + driverFactor * edge.dailyDriver;
    if (edge.crashProb > 1) {
        edge.crashProb = 1;
    } else if (edge.crashProb < 0) {
        edge.crashProb = 0;
    }
    return id;
}

void RoadGraph::recalculateProbAll() {
    for (std::size_t i = 0; i < edges_.size(); ++i) {
        recalculateProb(i, 0.00000001, 0.00000001, 0);
    }
}

std::vector<std::size_t> RoadGraph::BFS(std::pair<double, double> p1, std::pair<double, double> p2) const {
    return BFS(findNearestNeighbor(p1), findNearestNeighbor(p2));
}

std::vector<std::size_t> RoadGraph::BFS(std::size_t id1, std::size_t id2) const {   
    if (id1 == id2) {
        return {id1};
    }
    std::queue<std::size_t> q;
    std::set<std::size_t> visited;
    std::map<std::size_t, std::size_t> prev_map;
    bool isConnected = false;
    q.push(id1);
    visited.insert(id1);
    while (!q.empty()) {
        std::size_t temp = q.front();
        q.pop();
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

std::vector<std::size_t> RoadGraph::getNeighbors(std::size_t id) const {
    std::vector<std::size_t> to_return;
    for (const std::size_t& edge : nodes_[id].edges) {
        if (edges_[edge].start == id) {
            to_return.push_back(edges_[edge].end);
        } else {
            to_return.push_back(edges_[edge].start);
        }
    }
    return to_return;
}

std::pair<std::vector<double>, std::vector<std::size_t>> RoadGraph::PrimMST(std::pair<double, double> p) const {
    std::size_t start = findNearestNeighbor(p);
    std::vector<double> dist(nodes_.size(), 2);
    std::vector<std::size_t> prev(nodes_.size(), static_cast<std::size_t>(-1));
    std::priority_queue<std::pair<double, size_t>> queue;
    std::vector<bool> visited(nodes_.size(), false);

    for (const RoadNode& node : nodes_) {
        queue.push({-2.0, node.id});
    }
    dist[start] = 0;
    queue.push({0, start});

    while (!queue.empty()) {
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

std::pair<std::vector<double>, std::vector<std::size_t>> RoadGraph::DijkstraTree(std::size_t start, std::size_t end) const {
    std::vector<double> dist(nodes_.size(), 2);
    std::vector<std::size_t> prev(nodes_.size(), static_cast<std::size_t>(-1));
    std::priority_queue<std::pair<double, size_t>> queue;
    std::vector<bool> visited(nodes_.size(), false);

    for (const RoadNode& node : nodes_) {
        queue.push({-2.0, node.id});
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

std::vector<std::size_t> RoadGraph::Dijkstra(std::size_t start, std::size_t end) const {
    std::list<std::size_t> list;
    std::vector<std::size_t> prev = DijkstraTree(start, end).second;
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

std::vector<std::size_t> RoadGraph::Dijkstra(std::pair<double, double> p, std::pair<double, double> q) const {
    std::size_t start = findNearestNeighbor(p);
    std::size_t end = findNearestNeighbor(q);
    return Dijkstra(p,q);
}

std::vector<std::vector<std::size_t>> RoadGraph::ComponentBFS(std::size_t start) const {
    std::vector<std::vector<std::size_t>> bfs(nodes_.size(), std::vector<std::size_t>());
    std::vector<bool> visited(nodes_.size(), false);
    visited[start] = true;
    std::queue<std::size_t> q;
    q.push(start);

    while (!q.empty()) {
        std::size_t v = q.front();
        q.pop();
        for (const std::size_t& w : getNeighbors(v)) {
            if (!visited[w]) {
                bfs[v].push_back(w);
                visited[w] = true;
                q.push(w);
            }
        }
    }
    return bfs;
}

void RoadGraph::BFSHelper(std::size_t start, std::vector<std::vector<std::size_t>>& bfs, std::vector<bool>& visited) const {
    visited[start] = true;
    std::queue<std::size_t> q;
    q.push(start);

    while (!q.empty()) {
        std::size_t v = q.front();
        q.pop();
        for (const std::size_t& w : getNeighbors(v)) {
            if (!visited[w]) {
                bfs[v].push_back(w);
                visited[w] = true;
                q.push(w);
            }
        }
    }
}

std::vector<std::vector<std::size_t>> RoadGraph::BFS() const {
    return BFS(0);
}

std::vector<std::vector<std::size_t>> RoadGraph::BFS(std::size_t start) const {
    std::vector<std::vector<std::size_t>> bfs(nodes_.size(), std::vector<std::size_t>());
    std::vector<bool> visited(nodes_.size(), false);
    BFSHelper(start, bfs, visited);
    for (std::size_t i = 0; i < nodes_.size(); ++i) {
        if (visited[i] == false) {
            BFSHelper(i, bfs, visited);
        }
    }
    return bfs;
}

std::vector<std::vector<std::size_t>> RoadGraph::BFS(std::pair<double, double> p) const {
    return BFS(findNearestNeighbor(p));
}

void RoadGraph::fillMissingTrafficData() {
    std::size_t count = 0;
    std::vector<bool> visited(nodes_.size(), false);
    while (count < nodes_.size()) {
        std::size_t start = static_cast<std::size_t>(-1);
        for (std::size_t i = 0; i < nodes_.size(); ++i) {
            if (visited[i] == false) {
                for (const std::size_t& edge : nodes_[i].edges) {
                    if (edges_[edge].dailyDriver != 0) {
                        start = i;
                        break;
                    }
                }
            }
        }
        if (start != static_cast<std::size_t>(-1)) {
            std::vector<std::vector<std::size_t>> bfs = ComponentBFS(start);
            std::queue<std::size_t> queue;
            queue.push(start);
            while (!queue.empty()) {
                std::size_t v = queue.front();
                queue.pop();
                visited[v] = true;
                ++count;
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
                for (const std::size_t& n : bfs[v]) {
                    queue.push(n);
                }
            }
        } else {
            break;
        }
    }
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

std::vector<RoadNode>& RoadGraph::getNodes() {
    return nodes_;
}

std::vector<RoadNode::RoadEdge>& RoadGraph::getEdges() {
    return edges_;
}

KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>* RoadGraph::getTree() {
    return tree_;
}

void RoadGraph::buildTree() {
    for (const RoadNode& node : nodes_) {
        road_entries_vec_.push_back({node.pos.first, node.pos.second});
    }
    tree_ = new KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>(2, road_entries_vec_, 10);
    tree_->index->buildIndex();
}
