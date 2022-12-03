#pragma once

#include "RoadNode.h"

#include <vector>
#include <list>
#include <string>

class RoadEdge {
    public:
        std::size_t id;
        RoadNode* start;
        RoadNode* end;
        double crashProb;
        double length;
        double numCrashes;
        double dailyDriver;
        RoadEdge() : id(0), start(nullptr), end(nullptr), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId) : id(argId), start(nullptr), end(nullptr), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId, RoadNode& argStart, RoadNode& argEnd) : id(argId), start(&argStart), end(&argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId, RoadNode& argStart, RoadNode& argEnd, double argCrashProb, double argLength) : id(argId), start(&argStart), end(&argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd) : id(argId), start(argStart), end(argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd, double argCrashProb, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
        RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(0), length(argLength), numCrashes(0), dailyDriver(0) {}
};