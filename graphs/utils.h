#pragma once

#include <string>
#include <vector>

/**
 * RoadNode class
*/
class RoadNode {
    public:
        /**
         * RoadEdge class
        */
        class RoadEdge {
            public:
                /**
                 * Internal representation of the RoadEdge's id, start, end, crash probability, length, number of crashes, and number of daily drivers
                 * 
                 * "id" is a size_t that stores the position of the RoadEdge in a RoadGraph's "edges_" vector
                 * "start" is a size_t that stores the position of the starting RoadNode in a RoadGraph's "nodes_" vector
                 * "end" is a size_t that store the position of the ending RoadNode in a RoadGraph's "nodes_" vector
                 * 
                 * "crashProb" is a double that stores the crash probability of the RoadEdge
                 * "length" is a double that stores the length of the RoadEdge
                 * "numCrashes" is a double that stores the approximate number of crashes on the RoadEdge
                 * "dailyDriver" is a double that stores the approximate number of dialy drivers on the RoadEdge 
                */
                std::size_t id;
                std::size_t start;
                std::size_t end;
                double crashProb;
                double length;
                double numCrashes;
                double dailyDriver;

                /**
                 * Defaulted empty constructor for RoadEdge
                */
                RoadEdge();

                /**
                 * Constructor for RoadEdge with a RoadEgde id of "argId"
                 * 
                 * @param argId A size_t storing the position of the RoadEdge in a RoadGraph's "edges_" vector
                */
                RoadEdge(std::size_t argId);

                /**
                 * Constructor for RoadEdge with a RoadEdge id of "argId", a starting RoadNode id of "argStart", and an ending RoadNode id of "argEnd"
                 * 
                 * @param argId A size_t storing the position of the RoadEdge in a RoadGraph's "edges_" vector
                 * @param argStart A size_t storing the position of the starting RoadNode in a RoadGraph's "nodes_" vector
                 * @param argEnd A size_t storing the position of the ending RoadNode in a RoadGraph's "nodes_" vector
                */
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd);
                
                /**
                 * Constructor for RoadEdge with a RoadEdge id of "argId", a starting RoadNode id of "argStart", an ending RoadNode id of "argEnd", and a length of "argLength"
                 * 
                 * @param argId A size_t storing the position of the RoadEdge in a RoadGraph's "edges_" vector
                 * @param argStart A size_t storing the position of the starting RoadNode in a RoadGraph's "nodes_" vector
                 * @param argEnd A size_t storing the position of the ending RoadNode in a RoadGraph's "nodes_" vector
                 * @param argLength A double storing the length of the RoadEdge
                */
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argLength);

                /**
                 * Constructor for RoadEdge with a RoadEdge id of "argId", a starting RoadNode id of "argStart", an ending RoadNode id of "argEnd", a length of "argLength", and a probability of a crash of "argCrashProb"
                 * 
                 * @param argId A size_t storing the position of the RoadEdge in a RoadGraph's "edges_" vector
                 * @param argStart A size_t storing the position of the starting RoadNode in a RoadGraph's "nodes_" vector
                 * @param argEnd A size_t storing the position of the ending RoadNode in a RoadGraph's "nodes_" vector
                 * @param argLength A double storing the length of the RoadEdge
                 * @param argCrashProb A double storing the probability of getting in a crash on the RoadEdge
                */
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argCrashProb, double argLength);
        };
        /**
        * Internal representation of the RoadEdge's id, start, end, crash probability, length, number of crashes, and number of daily drivers
        * 
        * "id" is a size_t that stores the position of the RoadEdge in a RoadGraph's "nodes_" vector
        * "pos" is a pair of doubles that stores the GPS position of the RoadNode of the form {longitude, latitude}
        * "edges" is a vector of size_t's that stores the positions of the incident RoadEdges in a RoadGraph's "edges_" vector
        * "name" is a string that stores that name of the road that the RoadNode lies on
        */
        std::size_t id;
        std::pair<double, double> pos;
        std::vector<std::size_t> edges;
        std::string name;

        /**
         * Constructor for RoadNode with a RoadNode id of "argId" and a position of "argPos"
         * 
         * @param argId A size_t that stores the position of the RoadNode in a RoadGraph's "nodes_" vector
         * @param argPos A pair of doubles that stores the GPS position of the RoadNode of the form {longitude, latitude}
        */
        RoadNode(std::size_t argId, std::pair<double, double> argPos);

        /**
         * Constructor for RoadNode with a RoadNode id of "argId", a position of "argPos", and a name "argName"
         * 
         * @param argId A size_t that stores the position of the RoadNode in a RoadGraph's "nodes_" vector
         * @param argPos A pair of doubles that stores the GPS position of the RoadNode of the form {longitude, latitude}
         * @param argName A string that stores the name of the road that the RoadNode lies on
        */
        RoadNode(std::size_t argId, std::pair<double, double> argPos, std::string argName);
};