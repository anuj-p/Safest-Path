#pragma once

#include "../data/reader.h"
#include "utils.h"
#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"

/**
 * RoadGraph class
*/
class RoadGraph {
    public:
        /**
         * Empty constuctor for RoadGraph
        */
        RoadGraph();

        /**
         * Constructor for RoadGraph to be used with Reader data
         *
         * @param road_entries A list of RoadEntry objects that store the road network to be made into a RoadGraph
         * @param traffic_entries A list of TrafficEntry objects that store daily driver data for roads in the road network
         * @param crash_entries A list of CrashEntry objects that store crash record for roads in the road network
        */
        RoadGraph(const std::list<ReaderUtils::RoadEntry>& road_entries, const std::list<ReaderUtils::TrafficEntry>& traffic_entries, const std::list<ReaderUtils::CrashEntry>& crash_entries);
       
        /**
         * Inserts RoadNode object into the "nodes_" vector
         *
         * @param pos A pair of doubles storing GPS coordinates of the RoadNode in the form {longitude, latitude}
         * @return A size_t storing the position of the RoadNode in the "nodes_" vector
        */
        std::size_t insertNode(std::pair<double, double> pos);

        /**
         * Inserts RoadNode object into the "nodes_" vector
         * 
         * @param pos A pair of doubles storing GPS coordinates of the RoadNode in the form {longitude, latitude}
         * @param name A string storing the name of the road "pos" lies on
         * @return A size_t storing the position of the RoadNode in the "nodes_" vector
        */
        std::size_t insertNode(std::pair<double, double> pos, std::string name);

        /**
         * Creates a RoadEdge from "nodes_[node1]" to "nodes_[node2]" with length "length"
         * Inserts the new RoadEdge into the "edges_" vector
         * Inserts the new RoadEdge's position in the "edges_" vector into the RoadNodes' incident edges vectors
         * 
         * @param node1 A size_t storing the position of the first RoadNode in the "nodes_" vector
         * @param node2 A size_t storing the position of the second RoadNode in the "nodes_" vector
         * @param length A double storing the length of the road between the first and second RoadNodes
         * @return A size_t storing the position of the new RoadEdge in the "edges_" vector
        */
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length);

        /**
         * Creates a RoadEdge from "nodes_[node1]" to "nodes_[node2]" with length "length" and crash probability "crashProb"
         * Inserts the new RoadEdge into the "edges_" vector
         * Inserts the new RoadEdge's position in the "edges_" vector into the RoadNodes' incident edges vectors
         * 
         * @param node1 A size_t storing the position of the first RoadNode in the "nodes_" vector
         * @param node2 A size_t storing the position of the second RoadNode in the "nodes_" vector
         * @param length A double storing the length of the road between the first and second RoadNodes
         * @param crashProb A double storing the crash probability on the road between the first and second RoadNodes
         * @return A size_t storing the position of the new RoadEdge in the "edges_" vector
        */
        std::size_t insertEdge(std::size_t node1, std::size_t node2, double length, double crashProb);

        /**
         * Adds "numCrashes" divided by the number of incident edges of the RoadNode closest to "pos"
         * to the number of crashes stored in each of incident edges of the RoadNode closest to "pos"
         * Recalculates the crash probabilities of all RoadEdges that were edited
         * 
         * @param pos A pair of doubles storing the GPS position of the crash of the form {longitude, latitude}
         * @param numCrashes A double storing the number of vehicles involved in the crash
         * @return A size_t storing the psoition of the closest RoadNode to "pos" in the "nodes_" vector
        */
        std::size_t addCrash(std::pair<double, double> pos, double numCrashes);

        /**
         * Adds "numCrashes" divided by the number of incident edges of the RoadNode closest to "pos"
         * to the crash probabilities of the incident edges of the RoadNode closest to "pos"
         * Does NOT recalculate the crash probabilities of all RoadEdges that were connected
         * 
         * @param pos A pair of doubles storing the GPS position of the crash of the form {longitude, latitude}
         * @param numCrashes A double storing the number of vehicles involved in the crash
         * @return A size_t storing the position of the closest RoadNode to "pos" in the "nodes_" vector
        */
        std::size_t addCrashNoRecalc(std::pair<double, double> pos, double numCrashes);

        /**
         * Recalculates the probability of an crash on RoadEdge "edges_[id]"
         * 
         * @param id A size_t storing the position of the RoadEdge in the "edges_" vector
         * @param constantFactor A double storing a value to be added to the crash probability of the RoadEdge
         * @param lengthFactor A double storing a value to be multiplied by the length of the RoadEdge and added to the crash probability of the RoadEdge
         * @param driverFactor A double storing a value to be multiplied by the number of drivers on the RoadEdge and added to the crash probability of the RoadEdge
         * @return A size_t storing the position of the RoadEdge in the "edges_" vector
        */
        std::size_t recalculateProb(std::size_t id, double constantFactor, double lengthFactor, double driverFactor);

        std::pair<std::vector<double>, std::vector<std::size_t>> PrimMST(std::pair<double, double> p) const;

        /**
         * Runs Dijkstra's algorithm to find the safest path between RoadNodes "nodes_[start]" and "nodes_[end]"
         * 
         * @param start A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @param end A size_t storing the position of the ending RoadNode in the "nodes_" vector
         * @return A pair of a vector of doubles and a vector of size_t's storing the probabilites and paths calculated by Dijkstra's algorithm
        */
        std::pair<std::vector<double>, std::vector<std::size_t>> DijkstraTree(std::size_t start, std::size_t end) const;

        /**
         * Runs Dijkstra's algorithm to find the safest path between the RoadNodes nearest "p" and "q"
         * 
         * @param p A pair of doubles storing the starting GPS position of the form {longitude, latitude}
         * @param q A pair of doubles storing the ending GPS position of the form {longitude, latitude}
         * @return A vector of size_t's storing the positions of the RoadNodes in the "nodes_" vector that makeup the safest path between the RoadNodes nearest "p" and "q"
        */
        std::vector<std::size_t> Dijkstra(std::pair<double, double> p, std::pair<double, double> q) const;

        /**
         * Runs Dijkstra's algorithm to find the safest path between RoadNodes "nodes_[start]" and "nodes_[end]"
         *
         * @param start A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @param end A size_t storing the position of the ending RoadNode in the "nodes_" vector
         * @return A vector of size_t's storing the position of the RoadNodes in the "nodes_" vector that makeup the safest path between "nodes_[start]" and "nodes_[end]"
        */
        std::vector<std::size_t> Dijkstra(std::size_t start, std::size_t end) const;

        /**
         * Recalculates the crash probabilities of all RoadEdges in "edges_"
        */
        void recalculateProbAll();

        /**
         * Runs the BFS algorithm to find the shortest path in terms of number of RoadEdges between the RoadNodes nearest "p1" and "p2"
         * 
         * @param p1 A pair of doubles storing the starting GPS position of the form {longitude, latitude}
         * @param p2 A pair of doubles storing the ending GPS position of the form {longitude, latitude}
         * @return A vector of size_t's storing the positions of the RoadNode's in the "nodes_" vector that makeup the shortest path in terms of number of RoadEdges between the RoadNodes nearest "p1" and "p2"
        */
        std::vector<std::size_t> BFSPath(std::pair<double, double> p1, std::pair<double, double> p2) const;

        /**
         * Runs the BFS algorithm to find the shortest path in terms of number of RoadEdges between the RoadNodes "nodes_[node1]" and "nodes_[node2]"
         * 
         * @param node1 A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @param node2 A size_t storing the position of the ending RoadNode in the "nodes_" vector
         * @return A vector of size_t's storing the positions of the RoadNode's in the "nodes_" vector that makeup the shortest path in terms of number of RoadEdges between the RoadNodes "nodes_[node1]" and "nodes_[node2]"
        */
        std::vector<std::size_t> BFSPath(std::size_t node1, std::size_t node2) const;

        /**
         * Runs the BFS algorithm on the connected component of RoadNode "nodes_[start]"
         * 
         * @param start A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @return A vector of vectors of size_t's storing the positions in the "nodes_" vector of the children of the nth RoadNode in the "nodes_" vector
        */
        std::vector<std::vector<std::size_t>> ComponentBFS(std::size_t start) const;

        /**
         * Runs the BFS algorithm on the RoadGraph
         * 
         * @return A vector of vectors of size_t's storing the positions of the children in the "nodes_" vector of the nth RoadNode in the "nodes_" vector
        */
        std::vector<std::vector<std::size_t>> BFS() const;

        /**
         * Runs the BFS algorithm with starting RoadNode "nodes_[start]"
         * 
         * @param start A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @return A vector of vectors of size_t's storing the positions of the children in the "nodes_" vector of the nth RoadNode in the "nodes_" vector
        */
        std::vector<std::vector<std::size_t>> BFS(std::size_t start) const;

        /**
         * Runs the BFS algorithm with starting RoadNode of the RoadNode nearest "p"
         * 
         * @param p A pair of doubles storing the starting GPS position of the form {longitude, latitude}
         * @return A vector of vectors of size_t's storing the positions of the children in the "nodes_" vector of the nth RoadNode in the "nodes_" vector
        */
        std::vector<std::vector<std::size_t>> BFS(std::pair<double, double> p) const;

        /**
         * Gets the neighbor RoadNodes of the RoadNode "nodes_[id]"
         * 
         * @param id A size_t storing the position of the RoadNode in "nodes_"
         * @return A vector of size_t's storing the positions of the neighbor RoadNodes in the "nodes_" vector
        */
        std::vector<std::size_t> getNeighbors(std::size_t id) const;

        /**
         * Find the nearest RoadNode to "coord"
         * 
         * @param coord A pair of doubles storing a GPS position of the form {longitude, latitude}
         * @return A size_t storing the position of the nearest RoadNode in the "nodes_" vector to "coord"
        */
        std::size_t findNearestNeighbor(const std::pair<double, double>& coord) const;

        /**
         * Gets the nodes vector
         * 
         * @return A vector of RoadNode objects
        */
        std::vector<RoadNode>& getNodes();

        /**
         * Gets the edges vector
         * 
         * @return A vector of RoadEdge objects
        */
        std::vector<RoadNode::RoadEdge>& getEdges();

        /**
         * Gets the KDTree
         * 
         * @return A pointer to a nanoflan KDTree
        */
        KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double>* getTree();

        /**
         * Destroys the RoadGraph object
         * Deletes the nanoflan KDTree
        */
        ~RoadGraph();

        /**
         * Copy-assigns "rhs" into "*this" RoadGraph
         * 
         * @param rhs A RoadGraph to be copy-assigned into "*this"
         * @return A RoadGraph, "*this"
        */
        RoadGraph& operator=(const RoadGraph& rhs);

        /**
         * Copy constructor
         * 
         * @param other A RoadGraph to be copy-constructed
        */
        RoadGraph(const RoadGraph& other);

        /**
         * Builds the nanoflan KDTree
        */
        void buildTree();

        /**
         * For all RoadEdges with a zero driver count, replaces the driver count of the RoadEdge by the average driver counts of all incident edges of the start and end RoadNodes that have nonzero driver counts
        */
        void fillMissingTrafficData();

        /**
         * Move-assigns "rhs" into "*this" RoadGraph
         * 
         * @param rhs A RoadGraph to be move-assigned into "*this"
         * @return A RoadGraph, "*this"
        */
        RoadGraph& operator=(RoadGraph&& rhs);

        /**
         * Move constructor
         * 
         * @param other A RoadGraph to be move-constructed
        */
        RoadGraph(RoadGraph&& other);

    private:
        /**
         * Internal representation of Node list, Edge list, and KDTree
         * 
         * "nodes_" is a vector that stores the RoadNodes of the RoadGraph
         * "edges_" is a vector that stores the RoadEdges of the RoadGraph
         * "road_entries_vec_" used by nanoflan for the KDTree
         * "tree_" is a nanoflan KDTree
        */
        std::vector<RoadNode> nodes_;
        std::vector<RoadNode::RoadEdge> edges_;
        std::vector<std::vector<double>> road_entries_vec_;
        KDTreeVectorOfVectorsAdaptor<std::vector<std::vector<double>>, double> *tree_;

        /**
         * Helper method for BFS
         * Runs a BFS on the connected component of the RoadNode "nodes_[start]" and adds it to "bfs"
         * Updates "visited" accordingly
         * 
         * @param start A size_t storing the position of the starting RoadNode in the "nodes_" vector
         * @param bfs A vector of vectors of size_t's storing the positions in the "nodes_" vector of the children of the nth RoadNode in the "nodes_" vector for all connected components already traversed
         * @param visited A vector of bools storing if the nth RoadNode in the "nodes_" vector has been visited
        */
        void BFSHelper(std::size_t start, std::vector<std::vector<std::size_t>>& bfs, std::vector<bool>& visited) const;
};