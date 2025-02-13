#include "nnue.h"
#include "bitboard.h"
#include "incbin.h"
#include "position.h"
#include "util.h"

#include <iostream>
#include <fstream>

INCBIN(EmbeddedNNUE, EvalFile);

namespace NNUE {

  struct Net {
    alignas(Alignment) int16_t FeatureWeights[KingBuckets][2][6][64][L1];
    alignas(Alignment) int16_t FeatureBiases[L1];
    alignas(Alignment) int8_t L1Weights[OutputBuckets][L1][L2];
    alignas(Alignment) float L1Biases[OutputBuckets][L2];
    alignas(Alignment) float L2Weights[OutputBuckets][L2][L3];
    alignas(Alignment) float L2Biases[OutputBuckets][L3];
    alignas(Alignment) float L3Weights[OutputBuckets][L3];
    alignas(Alignment) float L3Biases[OutputBuckets];
  };

  Net* Weights;
  NNZEntry nnzTable[256];

  bool needRefresh(Color side, Square oldKing, Square newKing) {
    return (oldKing & 0b100) != (newKing & 0b100) ||
           KingBucketsScheme[relative_square(side, oldKing)] != 
           KingBucketsScheme[relative_square(side, newKing)];
  }

  void Accumulator::addPiece(Square kingSq, Color side, Piece pc, Square sq) {
    multiAdd<L1>((VecI*) colors[side], (VecI*) colors[side], 
      (VecI*) Weights->FeatureWeights[KingBucketsScheme[relative_square(side, kingSq)]][side != piece_color(pc)][piece_type(pc)-1][relative_square(side, sq)]);
  }

  void Accumulator::removePiece(Square kingSq, Color side, Piece pc, Square sq) {
    multiSub<L1>((VecI*) colors[side], (VecI*) colors[side], 
      (VecI*) Weights->FeatureWeights[KingBucketsScheme[relative_square(side, kingSq)]][side != piece_color(pc)][piece_type(pc)-1][relative_square(side, sq)]);
  }

  void Accumulator::reset(Color side) {
    memcpy(colors[side], Weights->FeatureBiases, sizeof(Weights->FeatureBiases));
  }

  void loadWeights() {
    Weights = (Net*) Util::allocAlign(sizeof(Net));
    memcpy(Weights, gEmbeddedNNUEData, sizeof(Net));
    for (int i = 0; i < 256; i++) {
      nnzTable[i].count = BitCount(i);
      Bitboard bits = i;
      int j = 0;
      while (bits)
        nnzTable[i].indexes[j++] = popLsb(bits);
    }
  }

  Score evaluate(Position& pos, Accumulator& accumulator) {
    int bucket = (BitCount(pos.pieces()) - 2) / ((32 + OutputBuckets - 1) / OutputBuckets);
    alignas(Alignment) uint8_t ftOut[L1];
    alignas(Alignment) float l1Out[L2];
    alignas(Alignment) float l2Out[L3];
    float l3Out;

    for (int them = 0; them <= 1; ++them) {
      int16_t* acc = accumulator.colors[pos.sideToMove ^ them];
      for (int i = 0; i < L1; i += I8InVec) {
        AsVecI(ftOut[them * L1 / 2 + i]) = 
          packusEpi16(mulhiEpi16(slliEpi16(minEpi16(maxEpi16(AsVecI(acc[i]), setzeroSi()), set1Epi16(NetworkQA)), 16 - FtShift), 
          minEpi16(AsVecI(acc[i + L1 / 2]), set1Epi16(NetworkQA))));
      }
    }

    for (int i = 0; i < L2; i += FloatInVec) {
      VecF vecBias = AsVecF(Weights->L1Biases[bucket][i]);
      AsVecF(l1Out[i]) = minPs(maxPs(mulAddPs(castEpi32ToPs(AsVecI(ftOut[i])), set1Ps(1.0f / float(NetworkQA * NetworkQA * NetworkQB >> FtShift)), vecBias), setzeroPs()), set1Ps(1.0f));
    }

    for (int i = 0; i < L3; i += FloatInVec) {
      VecF sums = setzeroPs();
      for (int j = 0; j < L2; j++)
        sums = mulAddPs(set1Ps(l1Out[j]), AsVecF(Weights->L2Weights[bucket][j][i]), sums);
      AsVecF(l2Out[i]) = minPs(maxPs(sums, setzeroPs()), set1Ps(1.0f));
    }

    VecF sums = setzeroPs();
    for (int i = 0; i < L3; i += FloatInVec)
      sums = mulAddPs(AsVecF(l2Out[i]), AsVecF(Weights->L3Weights[bucket][i]), sums);
    l3Out = reduceAddPs(sums) + Weights->L3Biases[bucket];

    return l3Out * NetworkScale;
  }
}
