//
// Created by filip on 4/3/19.
//

#include <stdbool.h>
#include "snake.h"
#include "../utils/colours.h"
#include "SDL2/SDL_rect.h"
#include "../core/drawing.h"


void drawSnakeSegment(Game *game, SnakeSegment segment)
{
    drawGameObject(game, segment.x, segment.y, COLOUR_SNAKE_SEGMENT);
}

Snake *getSnake(int x, int y, SegmentDirection direction)
{
    Snake *snake = malloc(sizeof(Snake));
    initSnake(snake, x, y, direction);
    return snake;
}


void initSnake(Snake *snake, int x, int y, SegmentDirection direction)
{
    snake->head = snake->tail = malloc(sizeof(SnakeSegment));
    snake->head->y = y;
    snake->head->x = x;
    snake->direction = direction;
    snake->head->next = NULL;
    snake->tail->next = NULL;
    snake->head->prev = NULL;
    snake->tail->prev = NULL;
    snake->length = 1;
    snake->addedSegment = false;
    snake->fitness = 0;
    snake->lifetime = 0;
    initFruit(&snake->fruit, snake);
    initNetwork(&snake->network);
    snake->isAlive = true;
}

void destroySnake(Snake *snake)
{
    destroySnakeSegments(snake);
    destroyNetwork(&snake->network);
}


void destroySnakeSegments(Snake *snake)
{
    SnakeSegment *currentSegment = snake->head;
    while (currentSegment)
    {
        SnakeSegment *next = currentSegment->next;
        free(currentSegment);
        currentSegment = next;
    }
}


void drawSnake(Snake *snake, Game *game)
{
    SnakeSegment *currentSegment = snake->head;
    while (currentSegment)
    {
        drawSnakeSegment(game, *currentSegment);
        currentSegment = currentSegment->next;
    }
    drawFruit(&snake->fruit, game);
}

void setSnakeDirection(Snake *snake, SegmentDirection direction)
{
    switch (direction)
    {

        case UP:
            if (snake->direction != DOWN)
                snake->direction = direction;
            break;
        case DOWN:
            if (snake->direction != UP)
                snake->direction = direction;
            break;
        case LEFT:
            if (snake->direction != RIGHT)
                snake->direction = direction;
            break;
        case RIGHT:
            if (snake->direction != LEFT)
                snake->direction = direction;
            break;
        default:
            break;
    }
}

void updateSnakePosition(Snake *snake)
{
    if (!snake->addedSegment)
    {
        switch (snake->direction)
        {
            case UP:
                snake->tail->y = snake->head->y - 1;
                snake->tail->x = snake->head->x;
                break;
            case DOWN:
                snake->tail->y = snake->head->y + 1;
                snake->tail->x = snake->head->x;
                break;
            case LEFT:
                snake->tail->y = snake->head->y;
                snake->tail->x = snake->head->x - 1;
                break;
            case RIGHT:
                snake->tail->y = snake->head->y;
                snake->tail->x = snake->head->x + 1;
                break;
            case NONE:
                break;
        }
        if (snake->length > 1)
        {
            SnakeSegment *tail = snake->tail;
            SnakeSegment *head = snake->head;
            if (tail->prev)
                tail->prev->next = NULL;
            snake->tail = tail->prev;
            head->prev = tail;
            if (head->next == tail)
                head->next = NULL;
            tail->next = head;
            tail->prev = NULL;
            snake->head = tail;
        }
    }
    snake->addedSegment = false;
    if (snake->isAlive)
        snake->lifetime++;
}

void addSnakeSegment(Snake *snake)
{
    SnakeSegment *newHead = malloc(sizeof(SnakeSegment));
    snake->addedSegment = true;
    switch (snake->direction)
    {
        case UP:
            newHead->y = snake->head->y - 1;
            newHead->x = snake->head->x;
            break;
        case DOWN:
            newHead->y = snake->head->y + 1;
            newHead->x = snake->head->x;
            break;
        case LEFT:
            newHead->y = snake->head->y;
            newHead->x = snake->head->x - 1;
            break;
        case RIGHT:
            newHead->y = snake->head->y;
            newHead->x = snake->head->x + 1;
            break;
        case NONE:
            break;
    }

    snake->head->prev = newHead;
    newHead->next = snake->head;
    snake->head = newHead;
    snake->head->prev = NULL;
    snake->length++;
}

void resetSnake(Snake *snake, int x, int y, SegmentDirection direction)
{
    destroySnakeSegments(snake);
    snake->head = snake->tail = malloc(sizeof(SnakeSegment));
    snake->head->y = y;
    snake->head->x = x;
    snake->direction = direction;
    snake->head->next = NULL;
    snake->tail->next = NULL;
    snake->head->prev = NULL;
    snake->tail->prev = NULL;
    snake->length = 1;
    snake->addedSegment = false;
}

bool isSnakeValid(Snake *snake)
{
    SnakeSegment *head = snake->head;
    SnakeSegment *currentSegment = head->next;

    if (snake->head->x >= BOARD_SIZE ||
        snake->head->y >= BOARD_SIZE ||
        snake->head->x < 0 ||
        snake->head->y < 0)
        return false;

    while (currentSegment)
    {
        if (currentSegment->x == head->x && currentSegment->y == head->y)
            return false;
        currentSegment = currentSegment->next;
    }
    return true;
}

bool isSnakeSegmentHere(Snake *snake, int x, int y)
{
    SnakeSegment *segment = snake->head;
    while (segment)
    {
        if (segment->x == x && segment->y == y)
            return true;
        segment = segment->next;
    }
    return false;
}

void calculateDistances(Fruit *fruit, Snake *snake)
{
    int leftUpWall = 1;
    int leftDownWall = 1;
    int rightDownWall = 1;
    int rightUpWall = 1;

    int leftSelf = 0;
    int rightSelf = 0;
    int upSelf = 0;
    int downSelf = 0;

    int leftUpSelf = 0;
    int leftDownSelf = 0;
    int rightUpSelf = 0;
    int rightDownSelf = 0;

    int leftFruit = 0;
    int rightFruit = 0;
    int upFruit = 0;
    int downFruit = 0;

    int leftUpFruit = 0;
    int leftDownFruit = 0;
    int rightUpFruit = 0;
    int rightDownFruit = 0;



    //self and fruit distance straight
    int tempX = snake->head->x - 1;
    int tempY = snake->head->y;
    int cnt = 1;

    while (tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftSelf == 0)
            leftSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && leftFruit == 0)
            leftFruit = 1;
        cnt++;
        tempX--;
    }

    if(leftSelf)
        leftSelf = 1 / leftSelf;

    tempX = snake->head->x + 1;
    tempY = snake->head->y;
    cnt = 1;

    while (tempX < BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightSelf == 0)
            rightSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && rightFruit == 0)
            rightFruit = 1;
        cnt++;
        tempX++;
    }
    if(rightSelf)
        rightSelf = 1 / rightSelf;

    tempX = snake->head->x;
    tempY = snake->head->y + 1;
    cnt = 1;

    while (tempY < BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && downSelf == 0)
            downSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && downFruit == 0)
            downFruit = 1;
        cnt++;
        tempY++;
    }
    if(downSelf)
        downSelf = 1 / downSelf;

    tempX = snake->head->x;
    tempY = snake->head->y - 1;
    cnt = 1;

    while (tempY >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && upSelf == 0)
            upSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && upFruit == 0)
            upFruit = 1;
        cnt++;
        tempY--;
    }

    if(upSelf)
        upSelf = 1 / upSelf;

    //wall, fruit and self distance diagonal
    tempX = snake->head->x - 1;
    tempY = snake->head->y - 1;
    cnt = 1;
    while (tempY >= 0 && tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftUpSelf == 0)
            leftUpSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && leftUpFruit == 0)
            leftUpFruit = 1;
        cnt++;
        leftUpWall++;
        tempY--;
        tempX--;
    }
    if (leftUpSelf)
        leftUpSelf = 1 / leftUpSelf;

    tempX = snake->head->x - 1;
    tempY = snake->head->y + 1;
    cnt = 1;
    while (tempY <= BOARD_SIZE && tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftDownSelf == 0)
            leftDownSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && leftDownFruit == 0)
            leftDownFruit = 1;

        cnt++;
        leftDownWall++;
        tempY++;
        tempX--;
    }
    if(leftDownSelf)
        leftDownSelf = 1 / leftDownSelf;

    tempX = snake->head->x + 1;
    tempY = snake->head->y + 1;
    cnt = 1;
    while (tempY <= BOARD_SIZE && tempX <= BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightDownSelf == 0)
            rightDownSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && rightDownFruit == 0)
            rightDownFruit = 1;
        cnt++;
        rightDownWall++;
        tempY++;
        tempX++;
    }
    if(rightDownSelf)
        rightDownSelf = 1 / rightDownSelf;

    tempX = snake->head->x + 1;
    tempY = snake->head->y - 1;
    cnt = 1;
    while (tempY >= 0 && tempX <= BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightUpSelf == 0)
            rightUpSelf = cnt;
        if (isFruitHere(fruit, tempX, tempY) && rightUpFruit == 0)
            rightUpFruit = 1;
        cnt++;
        rightUpWall++;
        tempY--;
        tempX++;
    }
    if(rightUpSelf)
        rightUpSelf = 1 / rightDownSelf;

    //wall distance straight
    setValue(&snake->network.layerIn, 12, 0, (double) 1 / (snake->head->y));
    setValue(&snake->network.layerIn, 23, 0, (double) 1 / (snake->head->x));
    setValue(&snake->network.layerIn, 17, 0, (double) 1 / (BOARD_SIZE - snake->head->x));
    setValue(&snake->network.layerIn, 15, 0, (double) 1 / (BOARD_SIZE - snake->head->y));

    setValue(&snake->network.layerIn, 16, 0, (double) 1 / leftUpWall);
    setValue(&snake->network.layerIn, 4, 0, (double) 1 / leftDownWall);
    setValue(&snake->network.layerIn, 11, 0, (double) 1 / rightUpWall);
    setValue(&snake->network.layerIn, 10, 0, (double) 1 / rightDownWall);

    setValue(&snake->network.layerIn, 18, 0, (double) upSelf);
    setValue(&snake->network.layerIn, 0, 0, (double) downSelf);
    setValue(&snake->network.layerIn, 21, 0, (double) leftSelf);
    setValue(&snake->network.layerIn, 9, 0, (double) rightSelf);

    setValue(&snake->network.layerIn, 7, 0, (double) upFruit);
    setValue(&snake->network.layerIn, 1, 0, (double) downFruit);
    setValue(&snake->network.layerIn, 14, 0, (double) leftFruit);
    setValue(&snake->network.layerIn, 2, 0, (double) rightFruit);

    setValue(&snake->network.layerIn, 13, 0, (double) leftUpSelf);
    setValue(&snake->network.layerIn, 19, 0, (double) leftDownSelf);
    setValue(&snake->network.layerIn, 5, 0, (double) rightUpSelf);
    setValue(&snake->network.layerIn, 20, 0, (double) rightDownSelf);

    setValue(&snake->network.layerIn, 6, 0, (double) leftUpFruit);
    setValue(&snake->network.layerIn, 8, 0, (double) leftDownFruit);
    setValue(&snake->network.layerIn, 22, 0, (double) rightUpFruit);
    setValue(&snake->network.layerIn, 3, 0, (double) rightDownFruit);
}


void calculateFitness(Snake *snake)
{
    snake->fitness = snake->lifetime * snake->lifetime * pow(2, snake->length);
}

int compareSnakeFitness(const void *s1, const void *s2)
{
    if ((*(Snake **) s1)->fitness > ((*(Snake **) s2)->fitness))
        return -1;
    if ((*(Snake **) s1)->fitness == ((*(Snake **) s2)->fitness))
        return 0;
    if ((*(Snake **) s1)->fitness < ((*(Snake **) s2)->fitness))
        return 1;
}









