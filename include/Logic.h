#ifndef LOGIC_H
#define LOGIC_H

#include "Defs.h"
#include <vector>

namespace Amazons {
namespace Logic {

void initBoard(Board &board);   // 用于初始化棋盘
bool inMap(const Point &point); // 用于判断是否在棋盘范围内
bool isEmpty(const Board &board, const Point &point); // 用于判断该点是否被占据
void applyMove(Board &board, const Move &move,
               int color); // 执行一步，改变棋盘状态
std::vector<Move> getLegalMoves(const Board &board,
                                int color); // 获取所有合法走法，返回一个vector

} // namespace Logic
} // namespace Amazons

#endif // !LOGIC_H
