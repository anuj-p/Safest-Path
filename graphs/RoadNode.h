#pragma once

#include "RoadEdge.h"
#include "Point.h"

#include <string>

class RoadNode {
    public:
        std::size_t id;
        Point pos;
        std::vector<RoadEdge::RoadEdge*> edges;
        std::string name;
        RoadNode() : id(0), pos(Point()), edges(), name("") {}
        RoadNode(Point argPos) : id(0), pos(argPos), edges(), name("") {}
        RoadNode(std::size_t argId) : id(argId), pos(Point()), edges(), name("") {}
        RoadNode(std::size_t argId, Point argPos) : id(argId), pos(argPos), edges(), name("") {}
        RoadNode(std::size_t argId, Point argPos, std::string argName) : id(argId), pos(argPos), edges(), name(argName) {}
        RoadNode(std::size_t argId, double argX, double argY) : id(argId), pos(Point(argX, argY)), edges(), name("") {}
        RoadNode(std::string argPos) : id(0), pos(), edges(), name("") {
            std::size_t loc = argPos.find_first_of(',');
            std::string xPos = argPos.substr(1,loc-1);
            std::string yPos = argPos.substr(loc+1,argPos.size()-loc-2);
            pos = Point(std::stod(xPos), std::stod(yPos));
        }
        RoadNode(std::size_t argId, std::string argPos) : id(argId), pos(), edges(), name("") {
            std::size_t loc = argPos.find_first_of(',');
            std::string xPos = argPos.substr(1,loc-1);
            std::string yPos = argPos.substr(loc+1,argPos.size()-loc-2);
            pos = Point(std::stod(xPos), std::stod(yPos));
        }
        bool operator==(const RoadNode& r) const {
            return pos.x == r.pos.x && pos.y == r.pos.y;
        };
        bool operator!=(const RoadNode& r) const {
            return !(*this == r);
        };
};
