#include "AI.h"
#include "Defs.h"
#include "Logic.h"
#include <vector>

namespace Amazons {
namespace AI {
const int MAX_DEPTH = 2;
Move think(const Board &board, int myColor) {
  std::vector<Move> moves = Logic::getLegalMoves(board, myColor);
  if (moves.empty())
    return {{-1, -1}, {-1, -1}, {-1, -1}};
  int bestScore = -1000000, alpha = -1000000, beta = 1000000;
  Move bestMove = moves[0];
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    Board tempBoard = board;
    Logic::applyMove(tempBoard, *it, myColor);
    int value;
    if (board.turnID == 1)
      value = minimax(tempBoard, 0, false, myColor, alpha, beta);
    else
      value = minimax(tempBoard, MAX_DEPTH - 1, false, myColor, alpha, beta);
    if (value > bestScore) {
      alpha = value;
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
int minimax(const Board &board, int depth, bool isMaximizing, int myColor,
            int alpha, int beta) {
  if (depth == 0) {
    return evaluate(board, myColor);
  }
  int currentColor = isMaximizing ? myColor : -myColor;
  std::vector<Move> moves = Logic::getLegalMoves(board, currentColor);
  if (moves.empty()) {
    return isMaximizing ? -1000000 : 1000000;
  }
  int score = isMaximizing ? -1000000 : 1000000;
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    Board tempBoard = board;
    Logic::applyMove(tempBoard, *it, currentColor);
    int value =
        minimax(tempBoard, depth - 1, !isMaximizing, myColor, alpha, beta);
    if (isMaximizing) {
      if (value > alpha)
        alpha = value;
      if (value > score)
        score = value;
      if (alpha >= beta)
        break;
    } else {
      if (value < beta)
        beta = value;
      if (value < score)
        score = value;
      if (beta <= alpha)
        break;
    }
  }
  return score;
}
} // namespace AI
} // namespace Amazons
