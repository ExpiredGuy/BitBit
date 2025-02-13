#pragma once

#include "types.h"

constexpr int CORRHIST_SIZE = 32768;
constexpr int CORRHIST_LIMIT = 1024;

using MainHistory = int16_t[COLOR_NB][SQUARE_NB * SQUARE_NB];
using CaptureHistory = int16_t[PIECE_NB * SQUARE_NB][PIECE_TYPE_NB];
using CounterMoveHistory = Move[PIECE_NB * SQUARE_NB];
using ContinuationHistory = int16_t[2][PIECE_NB * SQUARE_NB][PIECE_NB * SQUARE_NB];
using PawnCorrHist = int16_t[2][CORRHIST_SIZE];
using NonPawnCorrHist = int16_t[2][CORRHIST_SIZE];

inline int getCorrHistIndex(Key pawnKey) {
  return pawnKey % CORRHIST_SIZE;
}

inline void addToCorrhist(int16_t& history, int value) {
  history += value - int(history) * abs(value) / CORRHIST_LIMIT;
}

inline void addToHistory(int16_t& history, int value) {
  history += value - int(history) * abs(value) / 16384;
}