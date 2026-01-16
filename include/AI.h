#ifndef AI_H
#define AI_H

#include "Defs.h"

namespace Amazons {
namespace AI {
Move think(const Board &board, int myColor);
int evaluate(const Board &board, int myColor);
int countMobility(const Board &board, int myColor);
int calQueenTerritory(const Board &board, int myColor,
                      int myMap[GRIDSIZE][GRIDSIZE],
                      int opMap[GRIDSIZE][GRIDSIZE]);
int calTrapPenalty(const Board &board, int myColor,
                   int myMap[GRIDSIZE][GRIDSIZE],
                   int opMap[GRIDSIZE][GRIDSIZE]);
int calPosValue(const Board &board, int myColor);
int minimax(const Board &board, int depth, bool isMaximizing, int myColor,
            int alpha, int beta);
} // namespace AI
} // namespace Amazons
#endif
