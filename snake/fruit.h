//
// Created by filip on 4/4/19.
//

#ifndef SNAKENETWORK_FRUIT_H
#define SNAKENETWORK_FRUIT_H


#include <stdbool.h>

typedef struct Snake Snake;
typedef struct Game Game;

typedef struct Fruit
{
    int x;
    int y;
}Fruit;

void initFruit(Fruit *fruit, Snake *snake);
bool updateFruit(Fruit *fruit, Snake *snake);
void drawFruit(Fruit *fruit, Game *game);
bool isFruitHere(Fruit *fruit, int x, int y);



#endif //SNAKENETWORK_FRUIT_H
