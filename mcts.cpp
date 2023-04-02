#include "mcts.h"
#include <iostream>

int MCTS::ucb_select(Node& n)
{
    int best;
    float best_ucb = std::numeric_limits<float>::min();
    float cur_ucb;

    int i = 0;
    for (auto& child: n.children)
    {
        if (child->visits == 0) return i;
        cur_ucb = child->value / child->visits + sqrt(2) * sqrt(log(n.visits)/child->visits);
        if (cur_ucb > best_ucb)
        {
            best_ucb = cur_ucb;
            best = i;
        }
        ++i;
    }
    return best;
}

std::shared_ptr<Node> MCTS::search(std::shared_ptr<Node> n)
{
    std::shared_ptr<Node> cur = n;
    while (!cur->children.empty()) cur = cur->children[ucb_select(*cur)];
    return cur;
}

void MCTS::expand(std::shared_ptr<Node> n)
{
    for (auto& m: n->game->get_moves())
    {
        auto game_cpy = n->game->copy();
        game_cpy->make_move(m);
        n->children.push_back(std::make_shared<Node>(game_cpy, n, std::move(m)));
    }
}

int MCTS::simulate(Node& n)
{
    auto game_cpy = n.game->copy();
    while (game_cpy->winner() == nullptr) game_cpy->make_move(game_cpy->get_random_move());
    return *(game_cpy->winner());
}

void MCTS::backpropigate(std::shared_ptr<Node> n, int winner)
{
    float reward = winner == -1 ? 0.5:(int)(n->game->turn()!=winner);
    while (n != nullptr)
    {
        ++(n->visits);
        n->value += reward;
        reward = 1-reward;
        n = n->parent.lock();
    }
}

void MCTS::rollout(std::shared_ptr<Node> root)
{
    auto leaf_node = search(root);
    expand(leaf_node);
    int winner = simulate(*leaf_node);
    backpropigate(leaf_node, winner);
}

std::shared_ptr<Action> MCTS::iter_search(std::shared_ptr<Game> game, unsigned int rollouts)
{
    auto root_node = std::make_shared<Node>(game, nullptr, nullptr);
    
    for (;rollouts --> 0;) rollout(root_node); // cursed
    std::cout << "Finished Rollouts" << std::endl;

    auto best_child = std::max_element(root_node->children.begin(), root_node->children.end(),
    [](const std::shared_ptr<Node>& n1, std::shared_ptr<Node>& n2){return n1->visits < n2->visits;});

    return (*best_child)->move;
}
