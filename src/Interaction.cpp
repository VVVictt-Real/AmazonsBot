#include "Interaction.h"
#include "Defs.h"
#include "Logic.h"
#include <iostream>

using namespace std;

namespace Amazons {
namespace Interaction {
int initAndRestore(Board &board) {
  int turnID;
  cin >> turnID;
  Logic::initBoard(board);
  int myColor = grid_white;
  for (int i = 1; i <= turnID; i++) {
    int x0, y0, x1, y1, x2, y2;
    cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
    if (x0 == -1)
      myColor = grid_black;
    else {
      Move move = {{x0, y0}, {x1, y1}, {x2, y2}};
      Logic::applyMove(board, move, -myColor);
    }
    if (i <= turnID - 1) {
      cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
      Move move = {{x0, y0}, {x1, y1}, {x2, y2}};
      Logic::applyMove(board, move, myColor);
    }
  }
  board.turnID = turnID;
  return myColor;
}

void outputMove(const Move &move) {
  printf("%d %d %d %d %d %d\n", move.start.x, move.start.y, move.target.x,
         move.target.y, move.arrow.x, move.arrow.y);
}
} // namespace Interaction
} // namespace Amazons
