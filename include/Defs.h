#ifndef DEFS_H
#define DEFS_H

#include <vector>

namespace Amazons {
const int GRIDSIZE = 8;
const int OBSTACLE = 2;
const int judge_black = 0;
const int judge_white = 1;
const int grid_black = 1;
const int grid_white = -1;
const int EMPTY = 0;

struct Point {
  int x, y;
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

struct Move {
  Point start;
  Point target;
  Point arrow;
};

struct Board {
  int grid[GRIDSIZE][GRIDSIZE];
  int turnID;
  Point blackPieces[4];
  Point whitePieces[4];
};
} // namespace Amazons

#endif // !DEFS_H
