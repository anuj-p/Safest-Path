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

    RoadNode::RoadEdge::RoadEdge() : id(0), start(0), end(0), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId) : id(argId), start(0), end(0), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd) : id(argId), start(argStart), end(argEnd), crashProb(0), length(0), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argCrashProb, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(argCrashProb), length(argLength), numCrashes(0), dailyDriver(0) {}
    RoadNode::RoadEdge::RoadEdge(std::size_t argId, std::size_t argStart, std::size_t argEnd, double argLength) : id(argId), start(argStart), end(argEnd), crashProb(), length(argLength), numCrashes(0), dailyDriver(0) {}

    RoadNode::RoadNode() : id(0), pos(Point()), edges(), name("") {}
    RoadNode::RoadNode(Point argPos) : id(0), pos(argPos), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId) : id(argId), pos(Point()), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId, Point argPos) : id(argId), pos(argPos), edges(), name("") {}
    RoadNode::RoadNode(std::size_t argId, Point argPos, std::string argName) : id(argId), pos(argPos), edges(), name(argName) {}
    RoadNode::RoadNode(std::size_t argId, double argX, double argY) : id(argId), pos(Point(argX, argY)), edges(), name("") {}