#include "Board.h"
#include "Defs.h"
#include "Player.h"
#include <algorithm>
#include <iostream>
#include <limits>

namespace Amazons {
Move humanPlayer::decideMove(const Board &board) {
  std::vector<Move> legalMoves = board.getLegalMoves(myColor);
  if (legalMoves.empty()) {
    std::cout << "你输了！" << std::endl;
    return MOVE_EXIT;
  }
  while (true) {
    std::cout << "请输入你的走法：(格式：x1 y1 x2 y2 x3 y3)" << std::endl;
    std::cout << "输入-1：暂停or存盘or退出:";
    int a[6];
    bool check = false;
    for (int i = 0; i < 6; i++) {
      int temp;
      while (!(std::cin >> temp)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      if (temp == -1) {
        std::cout << "输入：" << std::endl;
        std::cout << "1:存盘并退出" << std::endl;
        std::cout << "2:直接退出" << std::endl;
        std::cout << "3.取消（返回棋局）" << std::endl;
        while (!(std::cin >> temp)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (temp == 1) {
          return MOVE_SAVE;
        }
        if (temp == 2) {
          return MOVE_EXIT;
        }
        check = true;
        break;
      }
      a[i] = temp;
    }
    if (check) {
      std::cout << "返回成功，请重新输入坐标：";
      continue;
    }
    Move m = {(int8_t)a[0], (int8_t)a[1], (int8_t)a[2],
              (int8_t)a[3], (int8_t)a[4], (int8_t)a[5]};
    if (std::find(legalMoves.begin(), legalMoves.end(), m) !=
        legalMoves.end()) {
      return m;
    } else {
      std::cout << "非法落子！请重新输入：";
    }
  }
}
} // namespace Amazons
