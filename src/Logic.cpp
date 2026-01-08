#include "Logic.h"
#include "Defs.h"
#include <cerrno>
#include <iostream>
#include <vector>

namespace Amazons {
namespace Logic {
static const int dx[] = {1, 0, -1, 0, 1, -1, 1, -1};
static const int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
void initBoard(Board &board) {
  for (int i = 0; i < GRIDSIZE; i++)
    for (int j = 0; j < GRIDSIZE; j++)
      board.grid[i][j] = EMPTY;
  board.grid[0][2] = board.grid[2][0] = board.grid[5][0] = board.grid[7][2] =
      grid_black;
  board.grid[0][5] = board.grid[7][5] = board.grid[2][7] = board.grid[5][7] =
      grid_white;
  board.blackPieces[0] = {0, 2};
  board.blackPieces[1] = {2, 0};
  board.blackPieces[2] = {5, 0};
  board.blackPieces[3] = {7, 2};
  board.whitePieces[0] = {0, 5};
  board.whitePieces[1] = {7, 5};
  board.whitePieces[2] = {2, 7};
  board.whitePieces[3] = {5, 7};
  board.turnID = 1;
}

bool inMap(const Point &point) {
  return (point.x >= 0 && point.x < GRIDSIZE && point.y >= 0 &&
          point.y < GRIDSIZE);
}

bool isEmpty(const Board &board, const Point &point) {
  return (board.grid[point.x][point.y] == EMPTY);
}

void applyMove(Board &board, const Move &move, int color) {
  board.grid[move.start.x][move.start.y] = EMPTY;
  board.grid[move.target.x][move.target.y] = color;
  board.grid[move.arrow.x][move.arrow.y] = OBSTACLE;
  Point *myPieces =
      (color == grid_black) ? board.blackPieces : board.whitePieces;
  for (int i = 0; i < 4; i++)
    if (myPieces[i] == move.start) {
      myPieces[i] = move.target;
      break;
    }
}

std::vector<Move>
getLegalMoves(const Board &board,
              int color) { // 存在问题：关于被原有的点挡住了的问题
  std::vector<Move> moves;
  const Point *pieces =
      (color == grid_black) ? board.blackPieces : board.whitePieces;
  for (int i = 0; i < 4; i++) {
    Point p = pieces[i];
    for (int dir = 0; dir < 8; dir++) {
      for (int step = 1;; ++step) {
        Point target_p = {static_cast<int8_t>(p.x + dx[dir] * step),
                          static_cast<int8_t>(p.y + dy[dir] * step)};
        if (!inMap(target_p) || !isEmpty(board, target_p))
          break;
        for (int obsDir = 0; obsDir < 8; ++obsDir) {
          for (int obsStep = 1;; ++obsStep) {
            Point arrow_p = {
                static_cast<int8_t>(target_p.x + dx[obsDir] * obsStep),
                static_cast<int8_t>(target_p.y + dy[obsDir] * obsStep)};
            if (!inMap(arrow_p))
              break;
            if (!isEmpty(board, arrow_p) && !(arrow_p == p))
              break;
            moves.push_back({p, target_p, arrow_p});
          }
        }
      }
    }
  }
  return moves;
}
void undoMove(Board &board, const Move &move, int color) {
  board.grid[move.arrow.x][move.arrow.y] = EMPTY;
  board.grid[move.target.x][move.target.y] = EMPTY;
  board.grid[move.start.x][move.start.y] = color;
  Point *myPieces =
      (color == grid_black) ? board.blackPieces : board.whitePieces;
  for (int i = 0; i < 4; i++)
    if (myPieces[i] == move.target) {
      myPieces[i] = move.start;
      break;
    }
}
} // namespace Logic
} // namespace Amazons
