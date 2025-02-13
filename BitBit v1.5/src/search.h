#pragma once

#include "history.h"
#include "nnue.h"
#include "position.h"
#include "types.h"
#include <mutex>


#include <condition_variable>
#include <vector>

namespace Search {

  struct Settings {
    int64_t time[COLOR_NB], inc[COLOR_NB], movetime, startTime;
    int movestogo, depth;
    uint64_t nodes;
    Position position;
    std::vector<uint64_t> prevPositions;
    Settings();
    inline bool standardTimeLimit() const { return time[WHITE] || time[BLACK]; }
  };

  struct SearchInfo {
    Score staticEval;
    Move playedMove, killerMove;
    bool playedCap;
    Move pv[MAX_PLY];
    int pvLength;
    int16_t* contHistory;
  };

  constexpr int SsOffset = 6;

  class Thread {
  public:
    std::mutex mutex;
    std::condition_variable cv;
    volatile bool searching = false, exitThread = false;
    volatile int completeDepth;
    volatile uint64_t nodesSearched, tbHits;

    Thread();
    void resetHistories();
    void idleLoop();

  private:
    int64_t optimumTime, maxTime;
    uint32_t maxTimeCounter;
    int rootDepth, ply = 0, keyStackHead, accumStackHead, pvIdx;
    Key keyStack[100 + MAX_PLY];
    NNUE::Accumulator accumStack[MAX_PLY];
    SearchInfo searchStack[MAX_PLY + SsOffset];
    RootMoveList rootMoves;
    MainHistory mainHistory;
    CaptureHistory captureHistory;
    ContinuationHistory contHistory;
    CounterMoveHistory counterMoveHistory;
    PawnCorrHist pawnCorrhist;
    NonPawnCorrHist wNonPawnCorrhist, bNonPawnCorrhist;
    NNUE::FinnyTable finny;
    Score searchPrevScore;

    void refreshAccumulator(Position& pos, NNUE::Accumulator& acc, Color side);
    void updateAccumulator(Position& pos, NNUE::Accumulator& acc);
    Score doEvaluation(Position& position);
    void sortRootMoves(int offset);
    bool visitRootMove(Move move);
    void playNullMove(Position& pos, SearchInfo* ss);
    void cancelNullMove();
    void playMove(Position& pos, Move move, SearchInfo* ss);
    void cancelMove();
    int getQuietHistory(Position& pos, Move move, SearchInfo* ss);
    int getCapHistory(Position& pos, Move move);
    int adjustEval(Position& pos, Score staticEval);
    void updateHistories(Position& pos, int bonus, int malus, Move bestMove,
                         Move* quietMoves, int quietCount, int depth, SearchInfo* ss);
    bool hasUpcomingRepetition(Position& pos, int ply);
    bool isRepetition(Position& pos, int ply);
    template<bool IsPV>
    Score qsearch(Position& position, Score alpha, Score beta, int depth, SearchInfo* ss);
    template<bool IsPV>
    Score negamax(Position& position, Score alpha, Score beta, int depth,
                  bool cutNode, SearchInfo* ss, Move excludedMove = MOVE_NONE);
    void startSearch();
  };

  template<bool root>
  int64_t perft(Position& pos, int depth);

  void initLmrTable();
  void init();
  void printInfo(int depth, int pvIdx, Score score, const std::string& pvString);
}
