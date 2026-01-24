#ifndef BOARD_H
#define BOARD_H

#include "Defs.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Amazons {
class Board {
private:
  int8_t grid[GRIDSIZE][GRIDSIZE];
  int turnID;
  Point blackPieces[4];
  Point whitePieces[4];

public:
  Board();                              // 用于初始化棋盘 1
  bool inMap(const Point &point) const; // 用于判断是否在棋盘范围内 1
  bool isEmpty(const Point &point) const; // 用于判断该点是否被占据 1
  int getGrid(const Point &point) const;
  int getTurnID() const;
  const Point *getPieces(int color) const;
  void applyMove(const Move &move,
                 int color); // 执行一步，改变棋盘状态 1
  std::vector<Move>
  getLegalMoves(int color) const; // 获取所有合法走法，返回一个vector 1
  int countLegalMoves(int color) const;       // 1
  void undoMove(const Move &move, int color); // 1
  int loadBoard(const std::string &filename); // 返回人类玩家的颜色
  void addTurnID() { turnID++; }
};
} // namespace Amazons
#endif // !BOARD_H
