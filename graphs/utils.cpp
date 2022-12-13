#include "utils.h"

RoadNode::RoadEdge::RoadEdge(std::size_t argId) : id(argId), start(0), end(0), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd) : id(argId), start(argStart), end(argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argCrashProb, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(), length(argLength), numCrashes(0), dailyDriver(0) {}

RoadNode::RoadNode(std::size_t argId, std::pair<double, double> argPos) : id(argId), pos(argPos), edges(), name("") {}
RoadNode::RoadNode(std::size_t argId, std::pair<double, double> argPos, std::string argName) : id(argId), pos(argPos), edges(), name(argName) {}
