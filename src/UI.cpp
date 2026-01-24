#include "UI.h"
#include "Board.h"
#include "Defs.h"
#include "Player.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace Amazons {
Game::Game() : blackPlayer(nullptr), whitePlayer(nullptr), humanColor(0) {}
Game::~Game() {
  if (blackPlayer)
    delete blackPlayer;
  if (whitePlayer)
    delete whitePlayer;
}
void Game::clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void Game::run() {
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  Player *human;
  Player *AI;
  while (true) {
    showMenu();
    int choice;
    cout << "请输入选项：";
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    switch (choice) {
    case 1:
      startNewGame();
      break;
    case 2:
      humanColor = board.loadBoard("saveGame.txt");
      if (humanColor == grid_black) {
        blackPlayer = new humanPlayer(grid_black);
        whitePlayer = new AIPlayer(grid_white);
      } else {
        whitePlayer = new humanPlayer(grid_white);
        blackPlayer = new AIPlayer(grid_black);
      }
      gameLoop(humanColor);
      break;
    case 3:
      cout << "Goodbye!" << endl;
      return;
    default:
      cout << "无效选项！" << endl;
      continue;
    }
  }
}

void Game::showMenu() {
  clearScreen();
  cout << "\n=========================" << endl;
  cout << "    亚马逊棋(Amazons)    " << endl;
  cout << "=========================" << endl;
  cout << "1.新游戏(New Game)" << endl;
  cout << "2.加载存档(Load Game)" << endl;
  cout << "3.结束(Exit)" << endl;
  cout << "=========================" << endl;
}

void Game::renderBoard() {
  clearScreen();
  cout << "\n当前回合：" << board.getTurnID() << endl;
  cout << "黑子：B     白子：W     障碍：X" << endl;
  cout << "你持" << ((humanColor == grid_black) ? "黑(B)" : "白(W)") << "子"
       << endl;
  cout << endl;
  cout << "    0   1   2   3   4   5   6   7  --> y" << endl;
  cout << "  ┌";
  for (int i = 0; i < 7; i++)
    cout << "───┬";
  cout << "───┐" << endl;

  for (int i = 0; i < 8; ++i) {
    // 2. 画中间的格子内容: │ . │ B │
    cout << i << " │"; // 行号 + 左边框
    for (int j = 0; j < 8; ++j) {
      int val = board.getGrid({static_cast<int8_t>(i), static_cast<int8_t>(j)});
      char symbol = ' ';
      if (val == grid_black)
        symbol = 'B'; // 或者用实心圆 ●
      else if (val == grid_white)
        symbol = 'W'; // 或者空心圆 ○
      else if (val == OBSTACLE)
        symbol = 'X';
      else
        symbol = ' ';

      cout << " " << symbol << " │"; // 棋子 + 右边框
    }
    cout << endl;

    // 3. 画行与行之间的分割线
    if (i < 7) {
      // 中间行: ├───┼───┤
      cout << "  ├";
      for (int k = 0; k < 7; k++)
        cout << "───┼";
      cout << "───┤" << endl;
    } else {
      // 最后一行底座: └───┴───┘
      cout << "  └";
      for (int k = 0; k < 7; k++)
        cout << "───┴";
      cout << "───┘" << endl;
    }
  }
  cout << "│" << endl << "│" << endl << "V x" << endl << endl;
}

void Game::gameLoop(int startColor) {
  int currentColor = startColor;
  while (true) {
    renderBoard();
    Move m;
    Player *currentPlayer =
        (currentColor == grid_black) ? blackPlayer : whitePlayer;
    m = currentPlayer->decideMove(board);
    if (m == MOVE_EXIT) {
      cout << "游戏结束" << endl;
      break;
    }
    if (m == MOVE_SAVE) {
      saveGame("saveGame.txt");
      cout << "已保存！按回车继续..." << endl;
      cin.get();
      cin.get();
      break;
    }
    board.applyMove(m, currentColor);
    if (currentColor == grid_white)
      board.addTurnID();
    currentColor = -currentColor;
  }
}

void Game::startNewGame() {
  board = Board();
  cout << "你想要选择黑（先手）还是白（后手）？" << endl;
  cout << "请输入：1(黑) or 2(白)" << endl;
  int choice;
  while (true) {
    while (!(cin >> choice)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (choice == 1 || choice == 2)
      break;
    cout << "请输入1或者2！" << endl;
    cout << "请输入：1(黑) or 2(白)" << endl;
  }
  humanColor = (choice == 1) ? grid_black : grid_white;
  if (humanColor == grid_black) {
    blackPlayer = new humanPlayer(grid_black);
    whitePlayer = new AIPlayer(grid_white);
  } else {
    whitePlayer = new humanPlayer(grid_white);
    blackPlayer = new AIPlayer(grid_black);
  }
  gameLoop(grid_black);
}

void Game::saveGame(const std::string &filename) {
  ofstream out;
  out.open(filename);
  if (!out) {
    cerr << "Error:无法保存存档！" << endl;
    return;
  }
  out << board.getTurnID() << endl;
  out << humanColor << endl;
  for (int i = 0; i < GRIDSIZE; i++) {
    for (int j = 0; j < GRIDSIZE; j++) {
      out << (int)board.getGrid(
                 {static_cast<int8_t>(i), static_cast<int8_t>(j)})
          << " ";
      // out << board.grid[i][j] << " ";
    }
    out << endl;
  }
  cout << "存档成功！" << endl;
}

void Game::loadGame(const std::string &filename) {
  humanColor = board.loadBoard(filename); // 返回人类玩家的颜色
}

} // namespace Amazons
