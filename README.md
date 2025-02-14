# BitBit Chess Engine

BitBit is a high-performance chess engine inspired by modern engine architectures, incorporating advanced search techniques such as NNUE and Monte Carlo Tree Search (MCTS).

---

## Credits

### ASCII Chess Board Representation
The ASCII chess board is Stockfish ASCII chess board.

### Engine Development
- BitBit is an independent project, but it takes inspiration from various open-source chess engines.
- The Monte Carlo Tree Search (MCTS) implementation is optimized for high-speed playouts in chess.
- NNUE evaluation is used to improve positional understanding.

---

## Features
- **Bitboard-based move generation** for efficient calculations.
- **NNUE (Efficiently Updatable Neural Network)** for strong positional play.
- **Monte Carlo Tree Search (MCTS)** to enhance search heuristics.
- **Zobrist hashing** for fast position recognition and transposition handling.
- **UCI protocol support**, making it compatible with chess GUIs such as Arena and CuteChess.

---

## Installation
### Compiling from Source
To build BitBit, use the following steps:

```sh
git clone https://github.com/your-username/BitBit.git
cd BitBit
make
