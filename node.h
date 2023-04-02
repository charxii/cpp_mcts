#ifndef NODE_H
#define NODE_H

#include "game.h"
#include <vector>
#include <memory>

class Node
{
public:
    std::shared_ptr<Game> game;
    std::shared_ptr<Action> move;
    std::weak_ptr<Node> parent;
    float 
    value;
    int visits; 
    std::vector<std::shared_ptr<Node>> children;

    Node(std::shared_ptr<Game> g, std::shared_ptr<Node> p, std::shared_ptr<Action> m)
    : game(g), parent(std::weak_ptr(p)), move(m), value(0), visits(0) {};
};

#endif