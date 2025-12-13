#include "AI.h"
#include "Defs.h"
#include "Interaction.h"
#include "Logic.h"

using namespace Amazons;

int main() {
  Board board;
  int myColor = Interaction::initAndRestore(board);
  Move move = AI::think(board, myColor);
  Interaction::outputMove(move);
  return 0;
}
