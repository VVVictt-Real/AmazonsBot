#ifndef UI_H
#define UI_H

#include "Board.h"
#include "Defs.h"
#include "Player.h"
#include <string>

namespace Amazons {
class Game {
public:
  void run();
  Game();
  ~Game();

private:
  Board board;
  Player *blackPlayer;
  Player *whitePlayer;
  int humanColor;

  void showMenu();               // 显示主菜单 1
  void startNewGame();           // 开始新游戏 1
  void gameLoop(int startColor); // 对战循环 1

  void renderBoard(); // 画棋盘 1

  void saveGame(const std::string &filename);
  void loadGame(const std::string &filename);

  void clearScreen();
};
} // namespace Amazons

#endif
