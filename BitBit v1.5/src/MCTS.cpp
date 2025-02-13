#include "mcts.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

Node::Node(Position p, Node* par) : pos(p), parent(par) {}

bool Node::isLeaf() const {
    return children.empty();
}

double Node::uctValue() const {
    return value / (visits + 1) + 1.41 * std::sqrt(std::log(parent ? parent->visits + 1 : 1) / (visits + 1));
}

MCTS::MCTS(Position rootPos, int iterations) : root(new Node(rootPos)), maxIterations(iterations) {}

Move MCTS::bestMove() {
    for (int i = 0; i < maxIterations; i++) {
        Node* node = select();
        double result = simulate(node->pos);
        backpropagate(node, result);
    }
    return selectBestChild(root)->pos.getLastMove();
}

Node* MCTS::select() {
    Node* node = root;
    while (!node->isLeaf())
        node = *std::max_element(node->children.begin(), node->children.end(),
                                 [](Node* a, Node* b) { return a->uctValue() < b->uctValue(); });
    if (node->visits > 0) expand(node);
    return node->isLeaf() ? node : node->children.front();
}

void MCTS::expand(Node* node) {
    std::vector<Move> moves = node->pos.generateLegalMoves();
    for (const auto& move : moves) {
        Position newPos = node->pos;
        newPos.makeMove(move);
        node->children.push_back(new Node(newPos, node));
    }
}

double MCTS::simulate(Position pos) {
    for (int depth = 0; depth < 50 && !pos.isGameOver(); depth++) {
        std::vector<Move> moves = pos.generateLegalMoves();
        if (moves.empty()) break;
        pos.makeMove(moves[rand() % moves.size()]);
    }
    return pos.evaluate();
}

void MCTS::backpropagate(Node* node, double result) {
    while (node) {
        node->visits++;
        node->value += result;
        node = node->parent;
    }
}

Node* MCTS::selectBestChild(Node* node) {
    return *std::max_element(node->children.begin(), node->children.end(),
                             [](Node* a, Node* b) { return a->visits < b->visits; });
}
