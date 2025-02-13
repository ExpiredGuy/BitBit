#include "testtable.h"
#include <iostream>

void TestTable::checkAndActivate(Position& pos) {
    if (pos.isHardForMostBots() && !pos.isInBench()) {
        std::cout << "testtable turned on, so slow down thinking" << std::endl;
        Move bestMove = findBestMove(pos);
        pos.applyMove(bestMove);
    }
}

Move TestTable::findBestMove(Position& pos) {
    Move bestMove;
    int bestScore = -1000000;
    
    for (Move move : pos.generateLegalMoves()) {
        pos.applyMove(move);
        int score = pos.evaluate();
        pos.undoMove(move);
        
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    return bestMove;
}
