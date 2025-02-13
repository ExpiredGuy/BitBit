#pragma once

#include "bitboard.h"
#include "move.h"
#include "nnue.h"
#include "types.h"
#include "zobrist.h"

void positionInit();

struct Threats {
  Bitboard byPawn, byMinor, byRook;
};

struct alignas(32) Position {
  Color sideToMove;
  Square epSquare;
  CastlingRights castlingRights;
  Bitboard byColorBB[COLOR_NB], byPieceBB[PIECE_TYPE_NB], blockersForKing[COLOR_NB], pinners[COLOR_NB], checkers;
  Piece board[SQUARE_NB];
  int halfMoveClock, gamePly;
  Key key, pawnKey, nonPawnKey[COLOR_NB];

  inline Bitboard pieces(PieceType pt) const { return byPieceBB[pt]; }
  inline Bitboard pieces(Color c) const { return byColorBB[c]; }
  inline Bitboard pieces(Color c, PieceType pt) const { return byColorBB[c] & byPieceBB[pt]; }
  inline Bitboard pieces(PieceType pt0, PieceType pt1) const { return byPieceBB[pt0] | byPieceBB[pt1]; }
  inline Bitboard pieces() const { return byColorBB[WHITE] | byColorBB[BLACK]; }
  inline Square kingSquare(Color c) const { return getLsb(pieces(c, KING)); }
  inline CastlingRights castlingRightsOf(Color c) const { return c == WHITE ? WHITE_CASTLING & castlingRights : BLACK_CASTLING & castlingRights; }
  inline bool hasNonPawns(Color c) const { return pieces(c) & ~pieces(PAWN, KING); }
  inline Bitboard attackersTo(Square sq, Color c) const { return attackersTo(sq, c, pieces()); }

  Bitboard attackersTo(Square, Bitboard) const;
  Bitboard attackersTo(Square, Color, Bitboard) const;
  Bitboard slidingAttackersTo(Square, Color, Bitboard) const;
  void updatePins(Color);
  void updateKeys();
  void updateAttacks();
  void removePiece(Square, Piece);
  void putPiece(Square, Piece);
  void movePiece(Square, Square, Piece);
  bool isQuiet(Move) const;
  bool is50mrDraw() const;
  bool isPseudoLegal(Move) const;
  bool isLegal(Move) const;
  void doNullMove();
  void doMove(Move, DirtyPieces&);
  void calcThreats(Threats&);
  Key keyAfter(Move) const;
  bool seeGe(Move, int) const;
  void setToFen(const std::string&);
  std::string toFenString() const;
};

std::ostream& operator<<(std::ostream&, Position&);
