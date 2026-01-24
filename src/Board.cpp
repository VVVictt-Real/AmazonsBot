#include "Board.h"
#include "Defs.h"
#include <cerrno>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace Amazons {
static const int dx[] = {1, 0, -1, 0, 1, -1, 1, -1};
static const int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
Board::Board() {
  for (int i = 0; i < GRIDSIZE; i++)
    for (int j = 0; j < GRIDSIZE; j++)
      grid[i][j] = EMPTY;
  grid[0][2] = grid[2][0] = grid[5][0] = grid[7][2] = grid_black;
  grid[0][5] = grid[7][5] = grid[2][7] = grid[5][7] = grid_white;
  blackPieces[0] = {0, 2};
  blackPieces[1] = {2, 0};
  blackPieces[2] = {5, 0};
  blackPieces[3] = {7, 2};
  whitePieces[0] = {0, 5};
  whitePieces[1] = {7, 5};
  whitePieces[2] = {2, 7};
  whitePieces[3] = {5, 7};
  turnID = 1;
}

bool Board::inMap(const Point &point) const {
  return (point.x >= 0 && point.x < GRIDSIZE && point.y >= 0 &&
          point.y < GRIDSIZE);
}

bool Board::isEmpty(const Point &point) const {
  return (grid[point.x][point.y] == EMPTY);
}

void Board::applyMove(const Move &move, int color) {
  grid[move.start.x][move.start.y] = EMPTY;
  grid[move.target.x][move.target.y] = color;
  grid[move.arrow.x][move.arrow.y] = OBSTACLE;
  Point *myPieces = (color == grid_black) ? blackPieces : whitePieces;
  for (int i = 0; i < 4; i++)
    if (myPieces[i] == move.start) {
      myPieces[i] = move.target;
      break;
    }
}

std::vector<Move> Board::getLegalMoves(int color) const {
  std::vector<Move> moves;
  const Point *pieces = (color == grid_black) ? blackPieces : whitePieces;
  for (int i = 0; i < 4; i++) {
    Point p = pieces[i];
    for (int dir = 0; dir < 8; dir++) {
      for (int step = 1;; ++step) {
        Point target_p = {static_cast<int8_t>(p.x + dx[dir] * step),
                          static_cast<int8_t>(p.y + dy[dir] * step)};
        if (!inMap(target_p) || !isEmpty(target_p))
          break;
        for (int obsDir = 0; obsDir < 8; ++obsDir) {
          for (int obsStep = 1;; ++obsStep) {
            Point arrow_p = {
                static_cast<int8_t>(target_p.x + dx[obsDir] * obsStep),
                static_cast<int8_t>(target_p.y + dy[obsDir] * obsStep)};
            if (!inMap(arrow_p))
              break;
            if (!isEmpty(arrow_p) && !(arrow_p == p))
              break;
            moves.push_back({p, target_p, arrow_p});
          }
        }
      }
    }
  }
  return moves;
}
int Board::countLegalMoves(
    int color) const { // 较快地计算数量，不考虑射箭，粗略估值
  int cntMoves = 0;
  const Point *pieces = (color == grid_black) ? blackPieces : whitePieces;
  for (int i = 0; i < 4; i++) {
    Point p = pieces[i];
    for (int dir = 0; dir < 8; dir++) {
      for (int step = 1;; ++step) {
        Point target_p = {static_cast<int8_t>(p.x + dx[dir] * step),
                          static_cast<int8_t>(p.y + dy[dir] * step)};
        if (!inMap(target_p) || !isEmpty(target_p))
          break;
        cntMoves++;
      }
    }
  }
  return cntMoves;
}
void Board::undoMove(const Move &move, int color) {
  grid[move.arrow.x][move.arrow.y] = EMPTY;
  grid[move.target.x][move.target.y] = EMPTY;
  grid[move.start.x][move.start.y] = color;
  Point *myPieces = (color == grid_black) ? blackPieces : whitePieces;
  for (int i = 0; i < 4; i++)
    if (myPieces[i] == move.target) {
      myPieces[i] = move.start;
      break;
    }
}
int Board::getGrid(const Point &point) const {
  return static_cast<int>(grid[point.x][point.y]);
}
int Board::getTurnID() const { return turnID; }
const Point *Board::getPieces(int color) const {
  return (color == grid_black) ? blackPieces : whitePieces;
}
int Board::loadBoard(const std::string &filename) {
  std::ifstream in;
  in.open(filename);
  if (!in) {
    std::cerr << "无法打开存档" << std::endl;
    return INFINITY;
  }
  int humanColor;
  in >> turnID;
  in >> humanColor;
  int countBlack = 0, countWhite = 0;
  for (int i = 0; i < GRIDSIZE; i++) {
    for (int j = 0; j < GRIDSIZE; j++) {
      int temp;
      in >> temp;
      grid[i][j] = (int8_t)temp;
      if (temp == grid_black)
        blackPieces[countBlack++] = {(int8_t)i, (int8_t)j};
      if (temp == grid_white)
        whitePieces[countWhite++] = {(int8_t)i, (int8_t)j};
      // board.grid[i][j] = temp;
      // if (temp == grid_white)
      //   board.whitePieces[countWhite++] = {i, j};
      // if (temp == grid_black)
      //   board.blackPieces[countBlack++] = {i, j};
    }
  }
  std::cout << "读盘成功" << std::endl;
  return humanColor;
}
} // namespace Amazons
