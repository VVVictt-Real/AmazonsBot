#include "Board.h"
#include "Defs.h"
#include "Interaction.h"
#include "Player.h"
#include "UI.h"

using namespace Amazons;

int main() {
#ifdef _BOTZONE_ONLINE
  Board board;
  int myColor = Interaction::initAndRestore(board);
  AIPlayer ai(myColor);
  Move move = ai.decideMove(board);
  Interaction::outputMove(move);
  return 0;
#else
  Game game;
  game.run();
  return 0;
#endif // _BOTZONE_ONLINE
}
