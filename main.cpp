#include "mcts.h"
#include "connect4.h"

int main()
{
    //std::shared_ptr<Game> 
    auto game = std::make_shared<Connect4>();
    MCTS agent;

    while (game->winner() == nullptr)
    {
        game->make_move(agent.iter_search(game, 16000));
        game->display();
    }

    std::cout << "The winner is " << *(game->winner()) << std::endl;
}