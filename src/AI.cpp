#include "AI.h"
#include "Defs.h"
#include "Logic.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <queue>
#include <vector>

namespace Amazons {
namespace AI {

const int INF = 0x3f3f3f3f;
clock_t startTime;
struct sortableMove {
  Move move;
  int score;
  bool operator<(const sortableMove &other) const {
    return score > other.score;
  }
};

// int calDepth(int moveNum) {
//   if (moveNum >= 1500)
//     return 1;
//   if (moveNum <= 200 && moveNum >= 50)
//     return 2;
//   if (moveNum <= 50)
//     return 3;
//   return 2;
// }

void bfs(const Board &board, int (*map)[8], int myColor) {
  Point q[100];
  const Point *myPieces =
      (myColor == grid_black) ? board.blackPieces : board.whitePieces;
  memset(map, 0x3f, sizeof(int) * 8 * 8);
  int dx[] = {1, 0, -1, 0, 1, 1, -1, -1};
  int dy[] = {0, 1, 0, -1, 1, -1, 1, -1};
  int head = 0, tail = 0;
  for (int i = 0; i < 4; i++) {
    q[tail++] = myPieces[i];
    map[myPieces[i].x][myPieces[i].y] = 0;
  }
  while (head < tail) {
    Point top = q[head];
    head++;
    int px = top.x, py = top.y;
    int step = map[px][py];
    for (int i = 0; i < 8; i++) {
      Point temp = {static_cast<int8_t>(px + dx[i]),
                    static_cast<int8_t>(py + dy[i])};
      if (Logic::inMap(temp) && Logic::isEmpty(board, temp) &&
          map[temp.x][temp.y] == INF) {
        map[temp.x][temp.y] = step + 1;
        q[tail++] = temp;
      }
    }
  }
}
Move think(const Board &board, int myColor) {
  startTime = clock();
  std::vector<Move> moves = Logic::getLegalMoves(board, myColor);
  if (moves.empty())
    return {{-1, -1}, {-1, -1}, {-1, -1}};
  std::vector<sortableMove> sortablemoves;
  Board tempBoard = board;
  for (const auto &m : moves) {
    Logic::applyMove(tempBoard, m, myColor);
    int score = evaluate(tempBoard, myColor);
    sortablemoves.push_back({{m}, score});
    Logic::undoMove(tempBoard, m, myColor);
  }
  std::sort(sortablemoves.begin(), sortablemoves.end());
  for (size_t i = 0; i < sortablemoves.size(); ++i) {
    moves[i] = sortablemoves[i].move;
  }
  Move bestMove = moves[0];
  // int MAX_DEPTH = calDepth(moves.size());
  for (int depth = 1; depth <= 4; depth++) {
    int alpha = -1000000, beta = 1000000, currentBestscore = -1000000;
    Move currentBest = bestMove;
    for (auto it = moves.begin(); it != moves.end(); ++it) {
      Logic::applyMove(tempBoard, *it, myColor);
      int value = minimax(tempBoard, depth - 1, false, myColor, alpha, beta);
      if (value > currentBestscore) {
        alpha = value;
        currentBestscore = value;
        currentBest = *it;
      }
      Logic::undoMove(tempBoard, *it, myColor);
    }
    bestMove = currentBest;
    double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (elapsed > 0.95)
      break;
  }
  return bestMove;
}

int evaluate(const Board &board, int myColor) {
  int myMoves = Logic::getLegalMoves(board, myColor).size();
  int opMoves = Logic::getLegalMoves(board, -myColor).size();
  return (myMoves - opMoves);
}

int evaluateTerritory(const Board &board, int myColor) {
  int mapBlack[8][8], mapWhite[8][8];
  bfs(board, mapBlack, grid_black);
  bfs(board, mapWhite, grid_white);
  int score = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board.grid[i][j] != EMPTY)
        continue;
      if (mapBlack[i][j] < mapWhite[i][j])
        score++;
      if (mapBlack[i][j] > mapWhite[i][j])
        score--;
    }
  }
  return myColor == grid_black ? score : -score;
}

int minimax(const Board &board, int depth, bool isMaximizing, int myColor,
            int alpha, int beta) {
  if (depth == 0) {
    if (board.turnID > 15)
      return evaluateTerritory(board, myColor);
    else
      return evaluate(board, myColor);
  }
  int currentColor = isMaximizing ? myColor : -myColor;
  std::vector<Move> moves = Logic::getLegalMoves(board, currentColor);
  if (moves.empty()) {
    return isMaximizing ? -1000000 : 1000000;
  }
  int score = isMaximizing ? -1000000 : 1000000;
  Board tempBoard = board;
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (elapsed > 0.95)
      return score;
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
    Logic::undoMove(tempBoard, *it, currentColor);
  }
  return score;
}
} // namespace AI
} // namespace Amazons
