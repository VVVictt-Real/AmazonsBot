#include "AI.h"
#include "Defs.h"
#include "Interaction.h"
#include "Logic.h"
#include "UI.h"

using namespace Amazons;

int main() {
#ifdef _BOTZONE_ONLINE
  Board board;
  int myColor = Interaction::initAndRestore(board);
  Move move = AI::think(board, myColor);
  Interaction::outputMove(move);
  return 0;
#else
  Game game;
  game.run();
  return 0;
#endif // _BOTZONE_ONLINE
}
