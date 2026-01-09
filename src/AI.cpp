#include "AI.h"
#include "Defs.h"
#include "Logic.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <queue>
#include <type_traits>
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
int posTable[8][8] = {
    {-5, -3, -1, -1, -1, -1, -3, -5}, {-3, 0, 1, 1, 1, 1, 0, -3},
    {-1, 1, 3, 3, 3, 3, 1, -1},       {-1, 1, 3, 4, 4, 3, 1, -1},
    {-1, 1, 3, 4, 4, 3, 1, -1},       {-1, 1, 3, 3, 3, 3, 1, -1},
    {-3, 0, 1, 1, 1, 1, 0, -3},       {-5, -3, -1, -1, -1, -1, -3, -5},
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

void queenBFS(const Board &board, int (*map)[8], int myColor) {
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
      for (int slide = 1;; slide++) {
        Point temp = {static_cast<int8_t>(px + dx[i] * slide),
                      static_cast<int8_t>(py + dy[i] * slide)};
        if (!Logic::inMap(temp))
          break;
        if (!Logic::isEmpty(board, temp))
          break;
        if (map[temp.x][temp.y] > step + 1) {
          map[temp.x][temp.y] = step + 1;
          q[tail++] = temp;
        }
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
    int score = countMobility(tempBoard, myColor);
    sortablemoves.push_back({{m}, score});
    Logic::undoMove(tempBoard, m, myColor);
  }
  std::sort(sortablemoves.begin(), sortablemoves.end());
  for (size_t i = 0; i < sortablemoves.size(); ++i) {
    moves[i] = sortablemoves[i].move;
  }
  Move bestMove = moves[0];
  // int MAX_DEPTH = calDepth(moves.size());
  for (int depth = 1; depth <= 20; depth++) {
    int alpha = -1000000, beta = 1000000, currentBestscore = -1000000;
    Move currentBest = bestMove;
    bool TimeOut = false;
    for (auto it = moves.begin(); it != moves.end(); ++it) {
      Logic::applyMove(tempBoard, *it, myColor);
      int value = minimax(tempBoard, depth - 1, false, myColor, alpha, beta);
      if (value > currentBestscore) {
        alpha = value;
        currentBestscore = value;
        currentBest = *it;
      }
      Logic::undoMove(tempBoard, *it, myColor);
      double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
      if (elapsed > 0.97) {
        TimeOut = true;
        break;
      }
    }
    if (!TimeOut)
      bestMove = currentBest;
    else
      break;
    double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (elapsed > 0.97)
      break;
  }
  return bestMove;
}

int countMobility(const Board &board, int myColor) {
  int myMoves = Logic::countLegalMoves(board, myColor);
  int opMoves = Logic::countLegalMoves(board, -myColor);
  return (myMoves - opMoves);
}

int calQueenTerritory(const Board &board, int myColor,
                      int myMap[GRIDSIZE][GRIDSIZE],
                      int opMap[GRIDSIZE][GRIDSIZE]) {
  int score = 0;
  for (int i = 0; i < GRIDSIZE; i++) {
    for (int j = 0; j < GRIDSIZE; j++) {
      if (board.grid[i][j] != EMPTY)
        continue;
      if (myMap[i][j] > opMap[i][j])
        score--;
      if (myMap[i][j] < opMap[i][j])
        score++;
    }
  }
  return score;
}
int calPosValue(const Board &board, int myColor) {
  int score = 0;
  const Point *myPieces =
      (myColor == grid_black) ? board.blackPieces : board.whitePieces;
  for (int i = 0; i < 4; i++) {
    score += posTable[myPieces[i].x][myPieces[i].y];
  }
  const Point *opPieces =
      (myColor == grid_black) ? board.whitePieces : board.blackPieces;
  for (int i = 0; i < 4; i++) {
    score -= posTable[opPieces[i].x][opPieces[i].y];
  }
  return score;
}
int calTrapPenalty(const Board &board, int myColor, int (*myMap)[8],
                   int (*opMap)[8]) {
  int dx[] = {1, 0, -1, 0, 1, 1, -1, -1};
  int dy[] = {0, 1, 0, -1, 1, -1, 1, -1};
  int score = 0;
  const Point *myPieces =
      (myColor == grid_black) ? board.blackPieces : board.whitePieces;
  int penalty[] = {-800, -200, -50, -5, 0, 0, 0, 0, 0};
  for (int i = 0; i < 4; i++) {
    Point p = myPieces[i];
    int liberties = 0;
    bool exitFlag = false;
    for (int j = 0; j < 8; j++) {
      Point temp = {static_cast<int8_t>(p.x + dx[j]),
                    static_cast<int8_t>(p.y + dy[j])};
      if (Logic::inMap(temp) && Logic::isEmpty(board, temp)) {
        liberties++;
        if (opMap[temp.x][temp.y] > myMap[temp.x][temp.y] + 1)
          exitFlag = true;
      }
    }
    if (!exitFlag)
      score += penalty[liberties];
  }
  return score;
}
int evaluate(const Board &board, int myColor) {
  int myDist[GRIDSIZE][GRIDSIZE];
  int opDist[GRIDSIZE][GRIDSIZE];
  queenBFS(board, myDist, myColor);
  queenBFS(board, opDist, -myColor);
  int mobilityScore = countMobility(board, myColor);
  int territoryScore = calQueenTerritory(board, myColor, myDist, opDist);
  int myPenaltyScore = calTrapPenalty(board, myColor, myDist, opDist);
  int opPenaltyScore = calTrapPenalty(board, -myColor, opDist, myDist);
  int posScore = calPosValue(board, myColor);
  double w_Mobility, w_Territory, w_Pos;
  double progress = (board.turnID <= 30) ? (double)board.turnID : 30.0;
  if (progress > 8.0)
    w_Pos = 0;
  else
    w_Pos = 2.0 - 2.0 * progress / 8.0;
  w_Territory = 0.1 + (10.0 - 0.1) * progress / 30.0;
  if (progress < 10.0)
    w_Mobility = 0.5 + (4.0 - 0.5) * (progress / 10.0);
  else
    w_Mobility = 4.0 - (progress - 10.0) / 20.0 * 4.0;
  double finalScore = (w_Territory * territoryScore) +
                      (w_Mobility * mobilityScore) + (w_Pos * posScore);
  finalScore += (myPenaltyScore - opPenaltyScore) / 2.0;
  return finalScore;
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
  Board tempBoard = board;
  for (auto it = moves.begin(); it != moves.end(); ++it) {
    double elapsed = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    if (elapsed > 0.97)
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
