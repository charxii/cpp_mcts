#ifndef MCTS_H
#define MCTS_H
#include "game.h"
#include "node.h"
#include <limits>
#include <cmath>
#include <algorithm>

class MCTS
{
    public:
    int ucb_select(Node& n);

    std::shared_ptr<Node> search(std::shared_ptr<Node> n);
    void expand(std::shared_ptr<Node> n);
    int simulate(Node& n);
    void backpropigate(std::shared_ptr<Node> n, int winner);
    void rollout(std::shared_ptr<Node> root);

    std::shared_ptr<Action> iter_search(std::shared_ptr<Game> game, unsigned int rollouts);
};

#endif