#ifndef UI_H
#define UI_H

#include "Defs.h"
#include <string>

namespace Amazons {
class Game {
public:
  void run();

private:
  Board board;
  int humanColor;
  int botColor;

  void showMenu();               // 显示主菜单 1
  void startNewGame();           // 开始新游戏 1
  void gameLoop(int startColor); // 对战循环 1

  void renderBoard(); // 画棋盘 1
  bool humanMove();   // 处理玩家输入
  bool aiMove();      // 处理AI思考

  void saveGame(const std::string &filename);
  void loadGame(const std::string &filename);

  void clearScreen();
};
} // namespace Amazons

#endif
