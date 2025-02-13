#ifndef TESTTABLE_H
#define TESTTABLE_H

#include "position.h"

class TestTable {
public:
    static void checkAndActivate(Position& pos);
private:
    static Move findBestMove(Position& pos);
};

#endif
