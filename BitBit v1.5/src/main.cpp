#include "cuckoo.h"
#include "threads.h"
#include "tt.h"
#include "uci.h"
#include "error_handler.h"
#include "MCTS.h"
#include "testtable.h"

int main() {
    try {
        
    } catch (const std::exception& e) {
        ErrorHandler::logError(e.what());
        return 1;
    }
    return 0;
}


#include <iostream>

int main(int argc, char** argv) {
  std::cout << "BitBit v1.5 " << engineVersion << " - ExpiredGuy" << std::endl;

  Zobrist::init();
  Bitboards::init();
  positionInit();
  Cuckoo::init();
  Search::init();
  UCI::init();

  Threads::setThreadCount(UCI::Options["threadnum"]);
  TT::resize(UCI::Options["hashco"]);
  NNUE::loadWeights();

  UCI::loop(argc, argv);
  Threads::setThreadCount(0);

  return 0;
}
