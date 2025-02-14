#include "movepick.h"
#include "tuning.h"
#include "uci.h"

MovePicker::MovePicker(SearchType _searchType, Position& _pos, Move _ttMove, Move _killerMove, Move _counterMove, MainHistory& _mainHist, CaptureHistory& _capHist, int _seeMargin, Search::SearchInfo* _ss) :
  searchType(_searchType), pos(_pos), ttMove(_ttMove), mainHist(_mainHist), capHist(_capHist), seeMargin(_seeMargin), ss(_ss) {
  stage = pos.checkers ? IN_CHECK_PLAY_TT : (_searchType == QSEARCH ? QS_PLAY_TT : PLAY_TT);
  if (stage == PLAY_TT) {
    killerMove = (_killerMove != _ttMove) ? _killerMove : MOVE_NONE;
    counterMove = (_counterMove != _ttMove && _counterMove != _killerMove) ? _counterMove : MOVE_NONE;
  }
  if (!pos.isPseudoLegal(ttMove)) ++stage;
}

int pieceTo(Position& pos, Move m) {
  return pos.board[move_from(m)] * SQUARE_NB + move_to(m);
}

Move_Score nextMove0(MoveList& moveList, int visitedCount) {
  int bestMoveI = visitedCount;
  for (int i = visitedCount + 1; i < moveList.size(); i++)
    if (moveList[i].score > moveList[bestMoveI].score)
      bestMoveI = i;
  std::swap(moveList[bestMoveI], moveList[visitedCount]);
  return moveList[visitedCount];
}

void MovePicker::scoreQuiets() {
  Threats threats;
  pos.calcThreats(threats);
  for (int i = 0; i < quiets.size();) {
    Move move = quiets[i].move;
    if (move == ttMove || move == killerMove || move == counterMove) {
      quiets.remove(i);
      continue;
    }
    int chIndex = pieceTo(pos, move);
    quiets[i++].score = mainHist[pos.sideToMove][move_from_to(move)] + capHist[chIndex][0];
  }
}

void MovePicker::scoreCaptures() {
  for (int i = 0; i < captures.size();) {
    Move move = captures[i].move;
    if (move == ttMove) {
      captures.remove(i);
      continue;
    }
    captures[i++].score = PIECE_VALUE[piece_type(pos.board[move_to(move)])] * 16;
  }
}

Move MovePicker::nextMove(bool skipQuiets) {
  while (true) {
    switch (stage) {
      case IN_CHECK_PLAY_TT:
      case QS_PLAY_TT:
      case PLAY_TT:
        ++stage;
        return ttMove;
      case GEN_CAPTURES:
        getStageMoves(pos, ADD_CAPTURES, &captures);
        scoreCaptures();
        ++stage;
        break;
      case PLAY_GOOD_CAPTURES:
        while (capIndex < captures.size()) {
          Move_Score move = nextMove0(captures, capIndex++);
          if (pos.seeGe(move.move, seeMargin)) return move.move;
          badCaptures.add(move);
        }
        ++stage;
        break;
      case PLAY_KILLER:
        ++stage;
        if (pos.isQuiet(killerMove) && pos.isPseudoLegal(killerMove)) return killerMove;
        break;
      case PLAY_COUNTER:
        ++stage;
        if (pos.isQuiet(counterMove) && pos.isPseudoLegal(counterMove)) return counterMove;
        break;
      case GEN_QUIETS:
        if (skipQuiets) { stage = PLAY_BAD_CAPTURES; break; }
        getStageMoves(pos, ADD_QUIETS, &quiets);
        scoreQuiets();
        ++stage;
        break;
      case PLAY_QUIETS:
        if (skipQuiets) { stage = PLAY_BAD_CAPTURES; break; }
        if (quietIndex < quiets.size()) return nextMove0(quiets, quietIndex++).move;
        ++stage;
        break;
      case PLAY_BAD_CAPTURES:
        if (badCapIndex < badCaptures.size()) return badCaptures[badCapIndex++].move;
        return MOVE_NONE;
    }
  }
}