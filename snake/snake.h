//
// Created by filip on 4/3/19.
//

#ifndef SNAKENETWORK_SNAKE_H
#define SNAKENETWORK_SNAKE_H

#include "stdbool.h"
#include "../neural_network/network.h"
#include "fruit.h"

typedef struct Game Game;

typedef enum SegmentDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
}SegmentDirection;

typedef struct SnakeSegment
{
    int x;
    int y;
    struct SnakeSegment* next;
    struct SnakeSegment* prev;
} SnakeSegment;

typedef struct Snake
{
    Network network;
    SnakeSegment* head;
    SnakeSegment* tail;
    SegmentDirection direction;
    Fruit fruit;
    int length;
    double fitness;
    int lifetime;
    bool addedSegment;
    bool isAlive;
} Snake;

void drawSnake(Snake *snake, Game *game);

void drawSnakeSegment(Game* game, SnakeSegment segment);

Snake *getSnake(int x, int y, SegmentDirection direction);

void initSnake(Snake *snake, int x, int y, SegmentDirection direction);

void setSnakeDirection(Snake *snake, SegmentDirection direction);

void updateSnakePosition(Snake *snake);

void destroySnake(Snake* snake);

void destroySnakeSegments(Snake *snake);

void addSnakeSegment(Snake* snake);

void resetSnake(Snake *snake, int x, int y, SegmentDirection direction);

bool isSnakeValid(Snake* snake);

void calculateDistances(Fruit *fruit, Snake *snake);

bool isSnakeSegmentHere(Snake *snake, int x, int y);

double getSnakeFitness(Snake* snake);

void calculateFitness(Snake* snake);

int compareSnakeFitness (const void * s1, const void * s2);


#endif //SNAKENETWORK_SNAKE_H
