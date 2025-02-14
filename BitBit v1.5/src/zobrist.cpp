#include "zobrist.h"
#include <random>

uint64_t ZOBRIST_TEMPO, ZOBRIST_PSQ[PIECE_NB][SQUARE_NB], 
         ZOBRIST_EP[FILE_NB], ZOBRIST_CASTLING[16], ZOBRIST_50MR[120];

namespace Zobrist {

  void init() {
    std::mt19937_64 gen(12345);
    std::uniform_int_distribution<uint64_t> dis;
    
    ZOBRIST_TEMPO = dis(gen);

    for (int pc = W_PAWN; pc < PIECE_NB; ++pc)
      for (Square sq = SQ_A1; sq < SQUARE_NB; ++sq)
        ZOBRIST_PSQ[pc][sq] = dis(gen);

    for (File f = FILE_A; f < FILE_NB; ++f)
      ZOBRIST_EP[f] = dis(gen);

    for (int i = 0; i < 16; i++)
      ZOBRIST_CASTLING[i] = (i && BitCount(i) > 1) ? 
        ((i & WHITE_OO ? ZOBRIST_CASTLING[WHITE_OO] : 0) ^
         (i & WHITE_OOO ? ZOBRIST_CASTLING[WHITE_OOO] : 0) ^
         (i & BLACK_OO ? ZOBRIST_CASTLING[BLACK_OO] : 0) ^
         (i & BLACK_OOO ? ZOBRIST_CASTLING[BLACK_OOO] : 0))
        : dis(gen) * (i > 0);

    memset(ZOBRIST_50MR, 0, sizeof(ZOBRIST_50MR));
    for (int i = 14; i <= 100; i += 8)
      std::fill_n(ZOBRIST_50MR + i, 8, dis(gen));
  }

}
