#pragma once

#include "simd.h"
#include "types.h"

using namespace SIMD;

struct Position;

struct SquarePiece {
  Square sq;
  Piece pc;
};

struct DirtyPieces {
  SquarePiece sub0, add0, sub1, add1;
  enum { NORMAL, CAPTURE, CASTLING } type;
};

namespace NNUE {

  constexpr int FeaturesWidth = 768;
  constexpr int L1 = 1280, L2 = 16, L3 = 32;
  constexpr int KingBuckets = 13, OutputBuckets = 8;
  constexpr int NetworkScale = 400, NetworkQA = 255, NetworkQB = 128;

  constexpr int KingBucketsScheme[64] = {
    0,  1,  2,  3,  3,  2,  1,  0,
    4,  5,  6,  7,  7,  6,  5,  4,
    8,  8,  9,  9,  9,  9,  8,  8,
    10, 10, 10, 10, 10, 10, 10, 10,
    11, 11, 11, 11, 11, 11, 11, 11, 
    11, 11, 11, 11, 11, 11, 11, 11, 
    12, 12, 12, 12, 12, 12, 12, 12, 
    12, 12, 12, 12, 12, 12, 12, 12, 
  };

  struct Accumulator {
    alignas(Alignment) int16_t colors[COLOR_NB][L1];
    bool updated[COLOR_NB];
    Square kings[COLOR_NB];
    DirtyPieces dirtyPieces;
    void addPiece(Square, Color, Piece, Square);
    void removePiece(Square, Color, Piece, Square);
    void doUpdates(Square, Color, Accumulator&);
    void reset(Color);
    void refresh(Position&, Color);
  };

  struct FinnyEntry {
    Bitboard byColorBB[COLOR_NB][COLOR_NB];
    Bitboard byPieceBB[COLOR_NB][PIECE_TYPE_NB];
    Accumulator acc;
    void reset();
  };

  using FinnyTable = FinnyEntry[2][KingBuckets];

  bool needRefresh(Color, Square, Square);
  void loadWeights();
  Score evaluate(Position&, Accumulator&);
}
