#include "AI.h"
#include "Defs.h"
#include "Logic.h"
#include <vector>

namespace Amazons {
namespace AI {
Move think(const Board &board, int myColor) {
  std::vector<Move> moves = Logic::getLegalMoves(board, myColor);
  if (moves.empty())
    return {{-1, -1}, {-1, -1}, {-1, -1}};
  int bestScore = -1000000;
  Move bestMove = moves[0];
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    Board tempBoard = board;
    Logic::applyMove(tempBoard, *it, myColor);
    int value = evaluate(tempBoard, myColor);
    if (value > bestScore) {
      bestScore = value;
      bestMove = *it;
    }
  }
  return bestMove;
}
int evaluate(const Board &board, int myColor) {
  int myMoves = Logic::getLegalMoves(board, myColor).size();
  int opMoves = Logic::getLegalMoves(board, -myColor).size();
  return (myMoves - opMoves);
}
} // namespace AI
} // namespace Amazons
