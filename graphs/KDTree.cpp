#include "KDTree.h"

KDTree::KDTree() : root(nullptr), size(0) {}

KDTree::KDTree(bool flag) : root(nullptr), size(0) {
    if (flag) {
        root = new KDTreeNode();
    }
}

KDTree::KDTree(const std::vector<KDPoint>& newPoints) : root(NULL), size(newPoints.size())
{
  if (size == 0) {
    return;
  }
  if (size == 1) {
    root = new KDTreeNode(newPoints.at(0));
    return;
  }
  std::vector<KDPoint> cpy(newPoints);
  buildTree(cpy, 0, size - 1, 0, root);
}

KDTree::KDTree(const std::vector<Point>& newPoints) {
  if (size == 0) {
    return;
  }
  std::vector<KDPoint> points;
  for(std::size_t i = 0; i < newPoints.size(); ++i) {
    points.push_back(KDPoint(i, newPoints[i]));
  }
  if (size == 1) {
    root = new KDTreeNode(points.at(0));
    return;
  }
  buildTree(points, 0, size - 1, 0, root);
}

KDTree::KDTree(std::vector<RoadNode>& newPoints) : root(NULL), size(newPoints.size())
{
  if (size == 0) {
    return;
  }
  std::vector<KDPoint> points;
  for(std::size_t i = 0; i < newPoints.size(); ++i) {
    points.push_back(KDPoint(newPoints[i].id, newPoints[i].pos));
  }
  if (size == 1) {
    root = new KDTreeNode(points.at(0));
    return;
  }
  buildTree(points, 0, size - 1, 0, root);
}

KDTree::KDTree(const KDTree& other) : root(NULL), size(other.size) {
  if (size == 0) {
    return;
  }
  root = new KDTreeNode(other.root->point);
  std::queue<KDTreeNode*> q;
  q.push(root);
  q.push(other.root->left);
  q.push(other.root->right);
  while (!q.empty()) {
    KDTreeNode* subroot = q.front();
    q.pop();
    KDTreeNode* child = q.front();
    q.pop();
    if (child) {
      subroot->left = new KDTreeNode(child->point);
      q.push(subroot->left);
      q.push(child->left);
      q.push(child->right);
    }
    child = q.front();
    q.pop();
    if (child) {
      subroot->right = new KDTreeNode(child->point);
      q.push(subroot->right);
      q.push(child->left);
      q.push(child->right);
    }
  }
}

const KDTree& KDTree::operator=(const KDTree& rhs) {
  if (this == &rhs) {
    return *this;
  }
  clear();
  root = new KDTreeNode(rhs.root->point);
  std::queue<KDTreeNode*> q;
  q.push(root);
  q.push(rhs.root->left);
  q.push(rhs.root->right);
  while (!q.empty()) {
    KDTreeNode* subroot = q.front();
    q.pop();
    KDTreeNode* child = q.front();
    q.pop();
    if (child) {
      subroot->left = new KDTreeNode(child->point);
      q.push(subroot->left);
      q.push(child->left);
      q.push(child->right);
    }
    child = q.front();
    q.pop();
    if (child) {
      subroot->right = new KDTreeNode(child->point);
      q.push(subroot->right);
      q.push(child->left);
      q.push(child->right);
    }
  }
  return *this;
}

KDTree::~KDTree() {
  clear();
}

bool KDTree::smallerDimVal(const KDPoint& first, const KDPoint& second, int curDim) const
{    
    if (curDim < 0 || curDim >= 2) {
      return first < second;
    }
    if (first[curDim] < second[curDim] || (first[curDim] == second[curDim] && first < second)) {
      return true;
    }
    return false;
}

bool KDTree::shouldReplace(const KDPoint& target, const KDPoint& currentBest, const KDPoint& potential) const
{
    uint64_t curDist = 0;
    uint64_t potDist = 0;
    for (std::size_t i = 0; i < 2; ++i) {
      curDist += (target[i] - currentBest[i]) * (target[i] - currentBest[i]);
      potDist += (target[i] - potential[i]) * (target[i] - potential[i]);
    }
    if (potDist < curDist || (potDist == curDist && potential < currentBest)) {
      return true;
    }
    return false;
}

KDTree::KDPoint KDTree::findNearestNeighbor(const KDPoint& query) const
{
  if (root) {
    return findNearestNeighborHelper(query, 0, root)->point;
  }
  return KDPoint();
}

KDTree::KDPoint KDTree::findNearestNeighbor(const Point& query) const {
  if (root) {
    return findNearestNeighborHelper(KDPoint(query), 0, root)->point;
  }
  return KDPoint();
}

KDTree::KDPoint KDTree::findNearestNeighbor(double x, double y) const {
  if (root) {
    return findNearestNeighborHelper(KDPoint(x, y), 0, root)->point;
  }
  return KDPoint();
}

std::size_t KDTree::partition(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t pivotIndex, std::size_t n) {
  KDPoint pivotValue = points.at(pivotIndex);

  KDPoint temp = points.at(pivotIndex);
  points.at(pivotIndex) = points.at(right);
  points.at(right) = temp;

  std::size_t storeIndex = left;

  for (std::size_t i = left; i < right; ++i) {
    if (smallerDimVal(points.at(i), pivotValue, n)) {
      temp = points.at(storeIndex);
      points.at(storeIndex) = points.at(i);
      points.at(i) = temp;
      ++storeIndex;
    }
  }

  temp = points.at(right);
  points.at(right) = points.at(storeIndex);
  points.at(storeIndex) = temp;

  return storeIndex;
}

KDTree::KDPoint KDTree::select(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t k, std::size_t n) {
  while (left != right) {
    std::size_t pivotIndex = (left + right) / 2;
    pivotIndex = partition(points, left, right, pivotIndex, n);
    if (k == pivotIndex) {
      return points.at(k);
    } else if (k < pivotIndex) {
      right = pivotIndex - 1;
    } else {
      left = pivotIndex + 1;
    }
  }
  return points.at(left);
}

int KDTree::buildTree(std::vector<KDPoint>& points, std::size_t left, std::size_t right, std::size_t n, KDTreeNode*& curRoot) {
  if (left > right) {
    return -1;
  }
    
  std::size_t middle = (left + right) / 2;
  KDPoint median = select(points, left, right, middle, n);
  curRoot = new KDTreeNode(median);

  if (middle != 0 && left <= middle - 1) {
    buildTree(points, left, middle - 1, (n + 1) % 2, curRoot->left);
  }
  if (middle < points.size() - 1 && middle + 1 <= right) {
    buildTree(points, middle + 1, right, (n + 1) % 2, curRoot->right);
  }
  return 0;
}

int KDTree::clear() {
  if (!root) {
    return 0;
  }
  std::queue<KDTreeNode*> q;
  q.push(root);
  while (!q.empty()) {
    KDTreeNode* subroot = q.front();
    q.pop();
    if (subroot->left) {
      q.push(subroot->left);
    }
    if (subroot->right) {
      q.push(subroot->right);
    }
    delete subroot;
  }
  return 0;
}

KDTree::KDTreeNode* KDTree::findNearestNeighborHelper(const KDPoint& query, std::size_t n, KDTreeNode* curRoot) const {
  if (curRoot->left == NULL && curRoot->right == NULL) {
    return curRoot;
  }
  if (curRoot->point == query) {
    return curRoot;
  }

  KDTreeNode* nearest = NULL;
  if (smallerDimVal(query, curRoot->point, n)) {
    nearest = curRoot->left ? findNearestNeighborHelper(query, (n + 1) % 2, curRoot->left) : curRoot;
  } else {
    nearest = curRoot->right ? findNearestNeighborHelper(query, (n + 1) % 2, curRoot->right) : curRoot;
  }

  if (shouldReplace(query, nearest->point, curRoot->point)) {
    nearest = curRoot;
  }

  double radius = 0;
  for (std::size_t i = 0; i < 2; ++i) {
    radius += (query[i] - nearest->point[i]) * (query[i] - nearest->point[i]);
  }

  double splitDist = (curRoot->point[n] - query[n]) * (curRoot->point[n] - query[n]);

  if (splitDist <= radius) {
    KDTreeNode* tempNearest = NULL;
    if (nearest == curRoot->left) {
      tempNearest = curRoot->right ? findNearestNeighborHelper(query, (n + 1) % 2, curRoot->right) : curRoot;
    } else {
      tempNearest = curRoot->left ? findNearestNeighborHelper(query, (n + 1) % 2, curRoot->left) : curRoot;
    }
    if (shouldReplace(query, nearest->point, tempNearest->point)) {
      nearest = tempNearest;
    }
  }
  return nearest;
}