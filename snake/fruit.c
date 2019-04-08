//
// Created by filip on 4/4/19.
//

#include "fruit.h"
#include "../core/game.h"
#include "stdlib.h"
#include "../utils/colours.h"
#include "../core/lifecycle.h"


bool updateFruit(Game *game)
{
    if(game->snake->head->x == game->fruit.x && game->snake->head->y == game->fruit.y)
    {
        game->fruit.x = rand()%BOARD_SIZE;
        game->fruit.y = rand()%BOARD_SIZE;
        return true;
    }
    return false;
}

void drawFruit(Game *game)
{
    drawGameObject(game, game->fruit.x, game->fruit.y, COLOUR_FRUIT);
}

bool isFruitHere(Game *game, int x, int y)
{
    return x == game->fruit.x && y == game->fruit.y;
}



