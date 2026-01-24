#ifndef INTERACTION_H
#define INTERACTION_H

#include "Board.h"
#include "Defs.h"

namespace Amazons {
namespace Interaction {
int initAndRestore(Board &board);
void outputMove(const Move &move);
} // namespace Interaction
} // namespace Amazons

#endif
