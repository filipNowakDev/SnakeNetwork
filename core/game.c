//
// Created by filip on 4/4/19.
//

#include "game.h"
#include "lifecycle.h"

void updateGameState(Game *game)
{
    if (updateFruit(game))
        addSnakeSegment(game->snake);
    updateSnakePosition(game->snake);
    if (!isSnakeValid(game->snake))
    {
        resetSnake(game->snake, BOARD_SIZE / 2, 3, DOWN);
        initRandomNetwork(game->network);
    }
}

bool isFruitHere(Game *game, int x, int y)
{
    return x == game->fruit.x && y == game->fruit.y;
}
