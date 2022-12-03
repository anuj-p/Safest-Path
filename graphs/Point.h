#pragma once

#include <compare>

class Point {
    public:
        double x;
        double y;
        bool operator==(const Point& p) const {
            return x == p.x && y == p.y;
        };
        bool operator!=(const Point& p) const {
            return !(*this == p);
        };
        bool operator>(const Point& p) const {
            return (x > p.x) || (x == p.x && y > p.y);
        };
        bool operator<(const Point& p) const {
            return (*this != p) && !(*this > p);
        };
        bool operator>=(const Point& p) const {
            return (*this == p) || (*this > p);
        };
        bool operator<=(const Point& p) const {
            return (*this == p) || (*this < p);
        };
        Point() : x(0), y(0) {}
        Point(double argX, double argY) : x(argX), y(argY) {}
        Point(const Point& p) : x(p.x), y(p.y) {}
        Point(std::pair<double, double> p) : x(p.first), y(p.second) {}
};