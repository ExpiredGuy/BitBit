#pragma once

#include "position.h"
#include <vector>

struct Node {
    Position pos;
    int visits = 0;
    double value = 0.0;
    std::vector<Node*> children;
    Node* parent = nullptr;

    Node(Position p, Node* par = nullptr);
    bool isLeaf() const;
    double uctValue() const;
};

class MCTS {
public:
    MCTS(Position rootPos, int iterations);
    Move bestMove();

private:
    Node* root;
    int maxIterations;

    Node* select();
    void expand(Node* node);
    double simulate(Position pos);
    void backpropagate(Node* node, double result);
    Node* selectBestChild(Node* node);
};
