#include "UI.h"
#include "AI.h"
#include "Defs.h"
#include "Logic.h"
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
      loadGame("saveGame.txt");
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
  cout << "\n当前回合：" << board.turnID << endl;
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
      int val = board.grid[i][j];
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
    if (currentColor == humanColor) {
      if (!humanMove())
        break;
    } else {
      if (!aiMove())
        break;
    }
    if (currentColor == grid_white)
      board.turnID++;
    currentColor = -currentColor;
  }
}

void Game::startNewGame() {
  Logic::initBoard(board);
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
  botColor = (choice == 1) ? grid_white : grid_black;
  gameLoop(grid_black);
}

bool Game::humanMove() {
  std::vector<Move> legalMoves = Logic::getLegalMoves(board, humanColor);
  if (legalMoves.empty()) {
    cout << "你输了！" << endl;
    return false;
  }
  while (true) {
    cout << "请输入你的走法：(格式：x1 y1 x2 y2 x3 y3)" << endl;
    cout << "输入-1：暂停or存盘or退出:";
    int a[6];
    bool check = false;
    for (int i = 0; i < 6; i++) {
      int temp;
      while (!(cin >> temp)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      if (temp == -1) {
        cout << "输入：" << endl;
        cout << "1:存盘并退出" << endl;
        cout << "2:直接退出" << endl;
        cout << "3.取消（返回棋局）" << endl;
        while (!(cin >> temp)) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (temp == 1) {
          saveGame("savegame.txt");
          return false;
        }
        if (temp == 2) {
          return false;
        }
        check = true;
        break;
      }
      a[i] = temp;
    }
    if (check) {
      renderBoard();
      continue;
    }
    Move m = {(int8_t)a[0], (int8_t)a[1], (int8_t)a[2],
              (int8_t)a[3], (int8_t)a[4], (int8_t)a[5]};
    if (std::find(legalMoves.begin(), legalMoves.end(), m) !=
        legalMoves.end()) {
      Logic::applyMove(board, m, humanColor);
      return true;
    } else {
      cout << "非法落子！请重新输入：";
    }
  }
}

bool Game::aiMove() {
  cout << "AI正在思考..." << endl;
  Move m = AI::think(board, botColor);
  if (m.start.x == -1) {
    cout << "你赢了！" << endl;
    return false;
  }
  Logic::applyMove(board, m, botColor);
  cout << "落子：(" << (int)m.start.x << "," << (int)m.start.y << ","
       << (int)m.target.x << "," << (int)m.target.y << "," << (int)m.arrow.x
       << "," << (int)m.arrow.y << ")" << endl;
  cout << "按回车键继续" << endl;
  cin.ignore();
  cin.get();
  return true;
}

void Game::saveGame(const std::string &filename) {
  ofstream out;
  out.open(filename);
  if (!out) {
    cerr << "Error:无法保存存档！" << endl;
    return;
  }
  out << board.turnID << endl;
  out << humanColor << endl;
  for (int i = 0; i < GRIDSIZE; i++) {
    for (int j = 0; j < GRIDSIZE; j++) {
      out << (int)board.grid[i][j] << " ";
      // out << board.grid[i][j] << " ";
    }
    out << endl;
  }
  cout << "存档成功！" << endl;
}

void Game::loadGame(const std::string &filename) {
  ifstream in;
  in.open(filename);
  if (!in) {
    cerr << "无法打开存档" << endl;
    return;
  }
  in >> board.turnID;
  in >> humanColor;
  botColor = -humanColor;
  int countBlack = 0, countWhite = 0;
  for (int i = 0; i < GRIDSIZE; i++) {
    for (int j = 0; j < GRIDSIZE; j++) {
      int temp;
      in >> temp;
      board.grid[i][j] = (int8_t)temp;
      if (temp == grid_black)
        board.blackPieces[countBlack++] = {(int8_t)i, (int8_t)j};
      if (temp == grid_white)
        board.whitePieces[countWhite++] = {(int8_t)i, (int8_t)j};
      // board.grid[i][j] = temp;
      // if (temp == grid_white)
      //   board.whitePieces[countWhite++] = {i, j};
      // if (temp == grid_black)
      //   board.blackPieces[countBlack++] = {i, j};
    }
  }
  cout << "读盘成功" << endl;
}

} // namespace Amazons
