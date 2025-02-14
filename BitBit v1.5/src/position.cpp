#include "position.h"
#include "move.h"
#include "movegen.h"
#include "uci.h"
#include <sstream>

CastlingRights ROOK_SQR_TO_CR[SQUARE_NB];

void positionInit() {
  for (Square sq = SQ_A1; sq <= SQ_H8; ++sq)
    ROOK_SQR_TO_CR[sq] = ALL_CASTLING;

  ROOK_SQR_TO_CR[SQ_A1] = ~WHITE_OOO;
  ROOK_SQR_TO_CR[SQ_H1] = ~WHITE_OO;
  ROOK_SQR_TO_CR[SQ_A8] = ~BLACK_OOO;
  ROOK_SQR_TO_CR[SQ_H8] = ~BLACK_OO;
  ROOK_SQR_TO_CR[SQ_E1] = ~WHITE_CASTLING;
  ROOK_SQR_TO_CR[SQ_E8] = ~BLACK_CASTLING;
}

Bitboard Position::attackersTo(Square s, Bitboard occupied) const {
  return  (getPawnAttacks(s, BLACK) & pieces(WHITE, PAWN)) |
          (getPawnAttacks(s, WHITE) & pieces(BLACK, PAWN)) |
          (getKnightAttacks(s) & pieces(KNIGHT)) |
          (getRookAttacks(s, occupied) | getBishopAttacks(s, occupied)) & pieces(QUEEN) |
          (getRookAttacks(s, occupied) & pieces(ROOK)) |
          (getBishopAttacks(s, occupied) & pieces(BISHOP)) |
          (getKingAttacks(s) & pieces(KING));
}

Bitboard Position::attackersTo(Square s, Color c, Bitboard occupied) const {
  return (getKnightAttacks(s) & pieces(KNIGHT)) |
         (getKingAttacks(s) & pieces(KING)) |
         (getBishopAttacks(s, occupied) | getRookAttacks(s, occupied)) & pieces(QUEEN) |
         (getBishopAttacks(s, occupied) & pieces(BISHOP)) |
         (getRookAttacks(s, occupied) & pieces(ROOK)) |
         (getPawnAttacks(s, ~c) & pieces(PAWN)) & pieces(c);
}

Bitboard Position::slidingAttackersTo(Square s, Color c, Bitboard occupied) const {
  return (getBishopAttacks(s, occupied) & pieces(BISHOP, QUEEN)) |
         (getRookAttacks(s, occupied) & pieces(ROOK, QUEEN)) & pieces(c);
}

void Position::updatePins(Color us) {
  Color them = ~us;
  blockersForKing[us] = pinners[them] = 0;
  Square ksq = kingSquare(us);
  Bitboard snipers = (getRookAttacks(ksq) & pieces(QUEEN, ROOK)) |
                     (getBishopAttacks(ksq) & pieces(QUEEN, BISHOP)) & pieces(them);
  Bitboard occupied = pieces() ^ snipers;
  
  while (snipers) {
    Square sniperSq = popLsb(snipers);
    Bitboard b = BETWEEN_BB[ksq][sniperSq] & occupied;
    if (BitCount(b) == 1) {
      blockersForKing[us] |= b;
      pinners[them] |= b & pieces(us) ? sniperSq : 0;
    }
  }
}
