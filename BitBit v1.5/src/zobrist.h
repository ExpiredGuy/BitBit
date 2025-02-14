#pragma once

#include "types.h"

extern uint64_t ZOBRIST_TEMPO, ZOBRIST_PSQ[PIECE_NB][SQUARE_NB], 
                ZOBRIST_EP[FILE_NB], ZOBRIST_CASTLING[16], ZOBRIST_50MR[120];

namespace Zobrist { void init(); }
