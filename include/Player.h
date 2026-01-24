#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Defs.h"

namespace Amazons {
class Player {
protected:
  int myColor;

public:
  Player(int c) : myColor(c) {}
  virtual ~Player() {}
  virtual Move decideMove(const Board &board) = 0;
  virtual bool isBot();
};
class AIPlayer : public Player {
private:
  void queenBFS(const Board &board, int (*map)[8], int isMyself);
  int evaluate(const Board &board);
  int countMobility(const Board &board);
  int calQueenTerritory(const Board &board, int myMap[GRIDSIZE][GRIDSIZE],
                        int opMap[GRIDSIZE][GRIDSIZE]);
  int calTrapPenalty(const Board &board, int isMyself,
                     int myMap[GRIDSIZE][GRIDSIZE],
                     int opMap[GRIDSIZE][GRIDSIZE]);
  int calPosValue(const Board &board);
  int minimax(const Board &board, int depth, bool isMaximizing, int alpha,
              int beta);

public:
  AIPlayer(int c) : Player(c) {}
  ~AIPlayer() {}
  Move decideMove(const Board &board);
  bool isBot() { return true; }
};
class humanPlayer : public Player {
public:
  humanPlayer(int c) : Player(c) {}
  ~humanPlayer() {}
  Move decideMove(const Board &board);
  bool isBot() { return false; }
};
} // namespace Amazons
#endif
