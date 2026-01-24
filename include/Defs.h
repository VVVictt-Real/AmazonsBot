#ifndef DEFS_H
#define DEFS_H

#include <cstdint>
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
  int8_t x, y;
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

struct Move {
  Point start;
  Point target;
  Point arrow;
  bool operator==(const Move &other) const {
    return start == other.start && target == other.target &&
           arrow == other.arrow;
  }
};
const Move MOVE_EXIT = {-1, -1, -1, -1, -1, -1};
const Move MOVE_SAVE = {-2, -2, -2, -2, -2, -2};
const Move MOVE_NULL = {-3, -3, -3, -3, -3, -3};
const Move MOVE_LOSE = {-4, -4, -4, -4, -4, -4};

} // namespace Amazons

#endif // !DEFS_H
