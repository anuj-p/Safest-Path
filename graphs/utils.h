#pragma once

#include <utility>
#include <string>
#include <vector>

class Point {
    public:
        double x;
        double y;
        Point();
        Point(double argX, double argY);
        Point(const Point& p);
        Point(std::pair<double, double> p);
        bool operator==(const Point& p) const;
        bool operator!=(const Point& p) const;
        bool operator>(const Point& p) const;
        bool operator<(const Point& p) const;
        bool operator>=(const Point& p) const;
        bool operator<=(const Point& p) const;
};

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
        Point pos;
        std::vector<std::size_t> edges;
        std::string name;
        RoadNode();
        RoadNode(Point argPos);
        RoadNode(std::size_t argId);
        RoadNode(std::size_t argId, Point argPos);
        RoadNode(std::size_t argId, Point argPos, std::string argName);
        RoadNode(std::size_t argId, double argX, double argY);
};