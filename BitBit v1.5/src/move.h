#pragma once

#include "types.h"

constexpr Move createMove(Square src, Square dest, MoveType moveType, int extra = 0) {
  return Move(src | (dest << 6) | (moveType << 12) | (extra << 14));
}

constexpr Move createPromoMove(Square src, Square dest, PieceType promoType) {
  return createMove(src, dest, MT_PROMOTION, promoType - KNIGHT);
}

constexpr Move createCastlingMove(CastlingRights type) {
  constexpr Move CastlingMoves[] = {
    MOVE_NONE, createMove(SQ_E1, SQ_G1, MT_CASTLING, 0), createMove(SQ_E1, SQ_C1, MT_CASTLING, 1),
    MOVE_NONE, createMove(SQ_E8, SQ_G8, MT_CASTLING, 2), MOVE_NONE, MOVE_NONE, MOVE_NONE,
    createMove(SQ_E8, SQ_C8, MT_CASTLING, 3)
  };
  return CastlingMoves[type];
}

inline Square move_from(Move move) { return Square(move & 63); }
inline Square move_to(Move move) { return Square((move >> 6) & 63); }
inline MoveType move_type(Move move) { return MoveType((move >> 12) & 3); }
inline int move_extra(Move move) { return move >> 14; }
inline PieceType promo_type(Move move) { return PieceType(move_extra(move) + KNIGHT); }
inline CastlingRights castling_type(Move move) { return CastlingRights(1 << move_extra(move)); }
inline bool isUnderPromo(Move m) { return move_type(m) == MT_PROMOTION && move_extra(m) != BISHOP; }
inline int move_from_to(Move m) { return m & 4095; }

struct Move_Score { Move move; int score; };
struct RootMove { Move move; int score; int nodes; Move pv[MAX_PLY]; int pvLength; };

struct MoveList {
  Move_Score moves[MAX_MOVES];
  int head = 0;

  inline void add(Move move) { moves[head++].move = move; }
  inline void add(Move_Score move) { moves[head++] = move; }
  inline int indexOf(Move move) const {
    for (int i = 0; i < head; i++) if (moves[i].move == move) return i;
    return -1;
  }
  inline int size() const { return head; }
  inline Move_Score& operator[](int index) { return moves[index]; }
  inline void remove(int index) { moves[index] = moves[--head]; }
  inline const Move_Score* begin() const { return moves; }
  inline const Move_Score* end() const { return moves + head; }
};

struct RootMoveList {
  RootMove moves[MAX_MOVES];
  int head = 0;

  inline void add(Move move) { moves[head++].move = move; }
  inline int indexOf(Move move) const {
    for (int i = 0; i < head; i++) if (moves[i].move == move) return i;
    return -1;
  }
  inline int size() const { return head; }
  inline RootMove& operator[](int index) { return moves[index]; }
  inline const RootMove* begin() const { return moves; }
  inline const RootMove* end() const { return moves + head; }
};
