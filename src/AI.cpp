#include "AI.h"
#include "Defs.h"
#include "Logic.h"
#include <cstdlib>
#include <ctime>
#include <vector>

namespace Amazons {
namespace AI {
Move think(const Board &board, int myColor) {
  srand(time(0));
  std::vector<Move> moves = Logic::getLegalMoves(board, myColor);
  if (moves.empty())
    return {{-1, -1}, {-1, -1}, {-1, -1}};
  int index = rand() % moves.size();
  return moves[index];
}
} // namespace AI
} // namespace Amazons
