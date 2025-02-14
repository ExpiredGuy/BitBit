#pragma once

#include "move.h"
#include "position.h"

enum MoveGenFlags { ADD_QUIETS = 1, ADD_CAPTURES = 2, ADD_ALL_MOVES = 3 };

void getStageMoves(const Position& pos, MoveGenFlags flags, MoveList* moveList);
void getQuietChecks(const Position& pos, MoveList* moveList);
