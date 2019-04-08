//
// Created by filip on 4/4/19.
//

#ifndef SNAKENETWORK_FRUIT_H
#define SNAKENETWORK_FRUIT_H


#include <stdbool.h>

typedef struct Game Game;

typedef struct Fruit
{
    int x;
    int y;
}Fruit;

bool updateFruit(Game *game);
void drawFruit(Game* game);
bool isFruitHere(Game * game, int x, int y);



#endif //SNAKENETWORK_FRUIT_H
