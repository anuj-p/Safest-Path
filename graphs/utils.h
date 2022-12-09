#pragma once

#include <utility>
#include <string>
#include <vector>

class RoadNode {
    public:
        class RoadEdge {
            public:
                std::size_t id;
                std::size_t start;
                std::size_t end;
                double crashProb;
                double length;
                double numCrashes;
                double dailyDriver;
                RoadEdge();
                RoadEdge(std::size_t argId);
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd);
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argCrashProb, double argLength);
                RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argLength);
        };
        std::size_t id;
        std::pair<double, double> pos;
        std::vector<std::size_t> edges;
        std::string name;
        RoadNode(std::size_t argId, std::pair<double, double> argPos);
        RoadNode(std::size_t argId, std::pair<double, double> argPos, std::string argName);
};