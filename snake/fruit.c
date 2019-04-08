//
// Created by filip on 4/4/19.
//

#include "fruit.h"
#include "../core/game.h"
#include "stdlib.h"
#include "../utils/colours.h"


bool updateFruit(Fruit *fruit, Snake *snake)
{
    if(snake->head->x == fruit->x && snake->head->y == fruit->y)
    {
        initFruit(fruit, snake);
        return true;
    }
    return false;
}

void drawFruit(Fruit *fruit, Game *game)
{
    drawGameObject(game, fruit->x, fruit->y, COLOUR_FRUIT);
}

bool isFruitHere(Fruit *fruit, int x, int y)
{
    return x == fruit->x && y == fruit->y;
}


void initFruit(Fruit *fruit, Snake *snake)
{
    do
    {
        fruit->x = rand() % BOARD_SIZE;
        fruit->y = rand() % BOARD_SIZE;
    }while(isSnakeSegmentHere(snake, fruit->x, fruit->y));
}



