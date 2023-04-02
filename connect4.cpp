#include "connect4.h"
#include "game.h"
#include <vector>
#include <memory>
#include <random>
#include <iostream>

Connect4::Connect4(): _turn(0), _winner_ptr(nullptr), board{0, 0}, dis(0, 6)
{
    std::random_device rd;
    mt.seed(rd());
};

Connect4::Connect4(const Connect4& c): _turn(c._turn), _winner(c._winner), board{c.board[0], c.board[1]}, dis(0, 6)
{
    std::random_device rd;
    mt.seed(rd());
    _winner_ptr = (c._winner_ptr == nullptr) ? nullptr:&_winner;
}

int Connect4::turn() { return _turn; }
int* Connect4::winner() { return _winner_ptr; }

std::vector<std::shared_ptr<Action>> Connect4::get_moves()
{
    if (_winner_ptr != nullptr) return {};

    uint64_t full = board[0]|board[1];
    std::vector<std::shared_ptr<Action>> legal;

    // Check if the top row is empty
    for (int i = HEIGHT-1; i < 48; i += HEIGHT+1)
    {
        if (((full>>i)&1) == 0)
        {
            std::shared_ptr<Action> action = std::make_shared<Connect4Action>(i/WIDTH);
            legal.push_back(action);
        }
    }
    return legal;
}

std::shared_ptr<Action> Connect4::get_random_move()
{
    int idx = dis(mt);

    uint64_t full = board[0]|board[1];
    for (int i = 0; i<7; ++i)
    {
        if (((full>>(idx*WIDTH+HEIGHT-1))&1) == 0)
        {
            // std::cout << idx << std::endl;
            std::shared_ptr<Action> a = std::make_shared<Connect4Action>(idx);
            return a;
        }
        idx = (idx+1) % WIDTH;
    }

    std::cout << "Move: " << idx << std::endl;
    std::cout << "Board ID: " << board[0] << ", " << board[1] << std::endl;
    display();
    throw std::invalid_argument("get_random_move() called on a full board");
}

void Connect4::make_move(int move) 
{
    std::shared_ptr<Action> action = std::make_shared<Connect4Action>(move);
    make_move(action);
}


void Connect4::make_move(std::shared_ptr<Action> move) 
{
    auto m = std::static_pointer_cast<Connect4Action>(move);
    int column = m->column;

    uint64_t combined = board[0]|board[1];
    uint64_t bottom = 1ull << (column*(HEIGHT+1));

    combined = combined | (combined+bottom);
    board[_turn] = board[1-_turn] ^ combined;

    _check_over();
    _turn = 1 - _turn;
}

void Connect4::_check_over()
{
    uint64_t pos = board[_turn];

    // Horizontal  >> 14 = 2 * height
    uint64_t m = pos & (pos >> HEIGHT+1);
    if (m & (m >> 2*(HEIGHT+1)))
    {
        _winner = _turn;
        _winner_ptr = &_winner;
        return;
    }

    // vertical
    m = pos & (pos >> 1);
    if (m & (m >> 2))
    {
        _winner = _turn;
        _winner_ptr = &_winner;
        return;
    }

    // \ Diagonal
    m = pos & (pos >> HEIGHT);
    if (m & (m >> 2*HEIGHT))
    {
        _winner = _turn;
        _winner_ptr = &_winner;
        return;
    }

    // / diagonal
    m = pos & (pos >> HEIGHT+2);
    if (m & (m >> 2*(HEIGHT+2)))
    {
        _winner = _turn;
        _winner_ptr = &_winner;
        return;
    }

    if ((board[0] | board[1]) == 0b111111011111101111110111111011111101111110111111)  // tie
    {
        _winner = -1;
        _winner_ptr = &_winner;
    }
};

std::shared_ptr<Game> Connect4::copy()
{
    return std::make_shared<Connect4>(*this);
}

void Connect4::display()
{
    char b[49];
    uint64_t red = board[0];
    uint64_t yel = board[1];

    for (int i=0; i<49; ++i)
    {
        if (red & 1) b[i] = 'O';
        else if (yel & 1) b[i] = 'X';
        else b[i] = '_';
        red = red >> 1;
        yel = yel >> 1;
    }

    /* [arr[(i * 7) + (7 - j - 1)] for j in range(1,7) for i in range(7)] */
    char rotatedArr[42];
    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            rotatedArr[(i-1)*7+j] = b[(j*7) + (6-i)];
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            std::cout << rotatedArr[i*7 + j] << "|";
        }
        std::cout << std::endl;
    }
}