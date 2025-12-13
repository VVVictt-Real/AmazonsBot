#ifndef AI_H
#define AI_H

#include "Defs.h"

namespace Amazons {
namespace AI {
Move think(const Board &board, int myColor);
int evaluate(const Board &board, int myColor);
} // namespace AI
} // namespace Amazons
#endif
