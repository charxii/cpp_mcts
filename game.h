#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>

class Action
{
    // The game's action should inheret this class
};

class Game
{
public:
    virtual int turn()=0;
    virtual int* winner()=0;
    virtual void make_move(std::shared_ptr<Action> move)=0;
    virtual std::shared_ptr<Game> copy()=0;
    virtual std::vector<std::shared_ptr<Action>> get_moves()=0;
    virtual std::shared_ptr<Action> get_random_move()=0;

    virtual void display()=0;  // only needs to be implemented for gui purposes
};

#endif