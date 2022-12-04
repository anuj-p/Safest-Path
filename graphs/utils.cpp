#include "utils.h"

    Point::Point() : x(0), y(0) {}
    Point::Point(double argX, double argY) : x(argX), y(argY) {}
    Point::Point(const Point& p) : x(p.x), y(p.y) {}
    Point::Point(std::pair<double, double> p) : x(p.first), y(p.second) {}
    bool Point::operator==(const Point& p) const {
        return x == p.x && y == p.y;
    };
    bool Point::operator!=(const Point& p) const {
        return !(*this == p);
    };
    bool Point::operator>(const Point& p) const {
        return (x > p.x) || (x == p.x && y > p.y);
    };
    bool Point::operator<(const Point& p) const {
        return (*this != p) && !(*this > p);
    };
    bool Point::operator>=(const Point& p) const {
        return (*this == p) || (*this > p);
    };
    bool Point::operator<=(const Point& p) const {
        return (*this == p) || (*this < p);
    };

    RoadNode::RoadEdge::RoadEdge() : id(0), start(nullptr), end(nullptr), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId) : id(argId), start(nullptr), end(nullptr), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, RoadNode& argStart, RoadNode& argEnd) : id(argId), start(&argStart), end(&argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, RoadNode& argStart, RoadNode& argEnd, double argCrashProb, double argLength) : id(argId), start(&argStart), end(&argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd) : id(argId), start(argStart), end(argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd, double argCrashProb, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, RoadNode* argStart, RoadNode* argEnd, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(0), length(argLength), numCrashes(0), dailyDriver(0) {}

    RoadNode::RoadNode() : id(0), pos(Point()), edges(), name("") {}
    RoadNode::RoadNode(Point argPos) : id(0), pos(argPos), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId) : id(argId), pos(Point()), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId, Point argPos) : id(argId), pos(argPos), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId, Point argPos, std::string argName) : id(argId), pos(argPos), edges(), name(argName) {}
    RoadNode::RoadNode(std::size_t argId, double argX, double argY) : id(argId), pos(Point(argX, argY)), edges(), name("") {}
    RoadNode::RoadNode(std::string argPos) : id(0), pos(), edges(), name("") {
        std::size_t loc = argPos.find_first_of(',');
        std::string xPos = argPos.substr(1,loc-1);
        std::string yPos = argPos.substr(loc+1,argPos.size()-loc-2);
        pos = Point(std::stod(xPos), std::stod(yPos));
    }
    RoadNode::RoadNode(std::size_t argId, std::string argPos) : id(argId), pos(), edges(), name("") {
        std::size_t loc = argPos.find_first_of(',');
        std::string xPos = argPos.substr(1,loc-1);
        std::string yPos = argPos.substr(loc+1,argPos.size()-loc-2);
        pos = Point(std::stod(xPos), std::stod(yPos));
    }