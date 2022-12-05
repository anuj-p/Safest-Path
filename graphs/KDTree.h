#pragma once

#include "utils.h"

#include <queue>

class KDTree {
  public:
    struct KDPoint {
        double x;
        double y;
        std::size_t node;
        std::size_t id;
        KDPoint() : x(0), y(0), node(0), id(0) {}
        KDPoint(Point argPoint) : x(argPoint.x), y(argPoint.y), node(0), id(0) {}
        KDPoint(std::size_t argId, Point argPoint) : x(argPoint.x), y(argPoint.y), node(argId), id(argId) {}
        KDPoint(std::size_t argId) : x(0), y(0), node(argId), id(argId) {}
        KDPoint(double argX, double argY) : x(argX), y(argY), node(0), id(0) {}
        KDPoint(std::size_t argId, double argX, double argY) : x(argX), y(argY), node(argId), id(argId) {}
        bool operator==(const KDPoint& rhs) const {
            return (x == rhs.x) && (y == rhs.y);
        }
        bool operator!=(const KDPoint& rhs) const {
            return !(*this == rhs);
        }
        bool operator>(const KDPoint& rhs) const {
            return (x > rhs.x) || (x == rhs.x && y > rhs.y);
        }
        bool operator<(const KDPoint& rhs) const {
            return (*this != rhs) && !(*this > rhs);
        }
        bool operator>=(const KDPoint& rhs) const {
            return (*this == rhs) || (*this > rhs);
        }
        bool operator<=(const KDPoint& rhs) const {
            return (*this == rhs) || (*this < rhs);
        }
        double operator[](std::size_t dim) const {
            if(dim % 2 == 0) {
                return x;
            }
            return y;
        }
        double& operator[](std::size_t dim) {
            if(dim % 2 == 0) {
                return x;
            }
            return y;
        }
    };
    KDTree();
    KDTree(bool flag);
    KDTree(const std::vector<KDPoint>& newPoints);
    KDTree(const std::vector<Point>& newPoints);
    KDTree(std::vector<RoadNode>& newPoints);
    KDTree(const KDTree& other);
    KDTree const &operator=(const KDTree& rhs);
    ~KDTree();
    bool smallerDimVal(const KDPoint& first, const KDPoint& second, int curDim) const;
    bool shouldReplace(const KDPoint& target, const KDPoint& currentBest, const KDPoint& potential) const;
    KDPoint findNearestNeighbor(const KDPoint& query) const;
    KDPoint findNearestNeighbor(const Point& query) const;
    KDPoint findNearestNeighbor(double x, double y) const;
  private:
    struct KDTreeNode
    {
        KDPoint point;
        KDTreeNode *left, *right;
        KDTreeNode() : point(), left(NULL), right(NULL) {}
        KDTreeNode(const KDPoint& point) : point(point), left(NULL), right(NULL) {}
    };
    KDTreeNode *root;
    std::size_t size;   
    std::size_t partition(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t pivotIndex, std::size_t n);
    KDPoint select(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t k, std::size_t n);
    int buildTree(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t n, KDTreeNode*& curRoot);
    int clear();
    KDTreeNode* findNearestNeighborHelper(const KDPoint& query, std::size_t n, KDTreeNode* curRoot) const;
};