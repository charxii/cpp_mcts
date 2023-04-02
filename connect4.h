#ifndef CONNECT4_H
#define CONNECT4_H

#include "game.h"
#include <vector>
#include <memory>
#include <random>
#include <iostream>

class Connect4Action: public Action
{
public:
    int column;
    Connect4Action(int c): column(c) {};
};

class Connect4: public Game
{
private:
    int _turn;
    int _winner;
    int* _winner_ptr;
    std::mt19937 mt;
    std::uniform_int_distribution<int> dis;
    
public:

    /* Colors also refer to the index they have in board */
    const static int RED = 0;
    const static int YELLOW = 1;
    const static int HEIGHT = 6;
    const static int WIDTH = 7;
    
    uint64_t board[2];

    Connect4();
    Connect4(const Connect4& c);

    int turn() override;
    
    /* Returns nullptr if the game has not been won yet. */
    int* winner() override; 

    /* Returns numbers 0-6 for each column that still has an empty space.*/
    std::vector<std::shared_ptr<Action>> get_moves() override;

    /* Iterates randomly over the board and returns the first legal move, Faster than get_moves if just one move is needed */
    std::shared_ptr<Action> get_random_move() override;

    /* Makes a move and updates the winner */
    void make_move(std::shared_ptr<Action> move) override;

    /* Makes a move and updates the winner, more useful for player */
    void make_move(int move);

    /* Helper function for make_move() */
    void _check_over();

    std::shared_ptr<Game> copy() override;

    void display() override;
};


#endif