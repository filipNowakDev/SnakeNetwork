//
// Created by filip on 4/3/19.
//

#include <stdbool.h>
#include "snake.h"
#include "../utils/colours.h"
#include "../core/lifecycle.h"
#include "SDL2/SDL_rect.h"
#include "../core/drawing.h"


void drawSnakeSegment(Game *game, SnakeSegment segment)
{
    drawGameObject(game, segment.x, segment.y, COLOUR_SNAKE_SEGMENT);
}

Snake *getSnake(int x, int y, SegmentDirection direction, Network *network)
{
    Snake *snake = malloc(sizeof(Snake));
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
    snake->network = network;
    return snake;
}

void destroySnake(Snake *snake)
{
    destroySnakeSegments(snake);
    free(snake);
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

void calculateDistances(Game *game, Snake *snake)
{
    int leftUpWall = 1;
    int leftDownWall = 1;
    int rightDownWall = 1;
    int rightUpWall = 1;

    int leftSelf = BOARD_SIZE;
    int rightSelf = BOARD_SIZE;
    int upSelf = BOARD_SIZE;
    int downSelf = BOARD_SIZE;

    int leftUpSelf = BOARD_SIZE;
    int leftDownSelf = BOARD_SIZE;
    int rightUpSelf = BOARD_SIZE;
    int rightDownSelf = BOARD_SIZE;

    int leftFruit = BOARD_SIZE;
    int rightFruit = BOARD_SIZE;
    int upFruit = BOARD_SIZE;
    int downFruit = BOARD_SIZE;

    int leftUpFruit = BOARD_SIZE;
    int leftDownFruit = BOARD_SIZE;
    int rightUpFruit = BOARD_SIZE;
    int rightDownFruit = BOARD_SIZE;

    //wall distance straight
    snake->network->layerIn[0] = (double) (snake->head->y) / BOARD_SIZE;
    snake->network->layerIn[1] = (double) (snake->head->x) / BOARD_SIZE;
    snake->network->layerIn[2] = (double) (BOARD_SIZE - snake->head->x) / BOARD_SIZE;
    snake->network->layerIn[3] = (double) (BOARD_SIZE - snake->head->y) / BOARD_SIZE;

    //self and fruit distance straight
    int tempX = snake->head->x - 1;
    int tempY = snake->head->y;
    int cnt = 1;

    while (tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftSelf == BOARD_SIZE)
            leftSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && leftFruit == BOARD_SIZE)
            leftFruit = cnt;
        cnt++;
        tempX--;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y;
    cnt = 1;

    while (tempX < BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightSelf == BOARD_SIZE)
            rightSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && rightFruit == BOARD_SIZE)
            rightFruit = cnt;
        cnt++;
        tempX++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y + 1;
    cnt = 1;

    while (tempY < BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && downSelf == BOARD_SIZE)
            downSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && downFruit == BOARD_SIZE)
            downFruit = cnt;
        cnt++;
        tempY++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y - 1;
    cnt = 1;

    while (tempY >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && upSelf == BOARD_SIZE)
            upSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && upFruit == BOARD_SIZE)
            upFruit = cnt;
        cnt++;
        tempY--;
    }


    //wall, fruit and self distance diagonal
    tempX = snake->head->x - 1;
    tempY = snake->head->y - 1;
    cnt = 1;
    while (tempY >= 0 && tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftUpSelf == BOARD_SIZE)
            leftUpSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && leftUpFruit == BOARD_SIZE)
            leftUpFruit = cnt;
        cnt++;
        leftUpWall++;
        tempY--;
        tempX--;
    }

    tempX = snake->head->x - 1;
    tempY = snake->head->y + 1;
    cnt = 1;
    while (tempY <= BOARD_SIZE && tempX >= 0)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && leftDownSelf == BOARD_SIZE)
            leftDownSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && leftDownFruit == BOARD_SIZE)
            leftDownFruit = cnt;

        cnt++;
        leftDownWall++;
        tempY++;
        tempX--;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y + 1;
    cnt = 1;
    while (tempY <= BOARD_SIZE && tempX <= BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightDownSelf == BOARD_SIZE)
            rightDownSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && rightDownFruit == BOARD_SIZE)
            rightDownFruit = cnt;
        cnt++;
        rightDownWall++;
        tempY++;
        tempX++;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y - 1;
    cnt = 1;
    while (tempY >= 0 && tempX <= BOARD_SIZE)
    {
        if (isSnakeSegmentHere(snake, tempX, tempY) && rightUpSelf == BOARD_SIZE)
            rightUpSelf = cnt;
        if (isFruitHere(game, tempX, tempY) && rightUpFruit == BOARD_SIZE)
            rightUpFruit = cnt;
        cnt++;
        rightUpWall++;
        tempY--;
        tempX++;
    }


    snake->network->layerIn[4] = (double) leftUpWall / BOARD_SIZE;
    snake->network->layerIn[5] = (double) leftDownWall / BOARD_SIZE;
    snake->network->layerIn[6] = (double) rightUpWall / BOARD_SIZE;
    snake->network->layerIn[7] = (double) rightDownWall / BOARD_SIZE;

    snake->network->layerIn[8] = (double) upSelf / BOARD_SIZE;
    snake->network->layerIn[9] = (double) downSelf / BOARD_SIZE;
    snake->network->layerIn[10] = (double) leftSelf / BOARD_SIZE;
    snake->network->layerIn[11] = (double) rightSelf / BOARD_SIZE;

    snake->network->layerIn[16] = (double) upFruit / BOARD_SIZE;
    snake->network->layerIn[17] = (double) downFruit / BOARD_SIZE;
    snake->network->layerIn[18] = (double) leftFruit / BOARD_SIZE;
    snake->network->layerIn[19] = (double) rightFruit / BOARD_SIZE;

    snake->network->layerIn[12] = (double) leftUpSelf / BOARD_SIZE;
    snake->network->layerIn[13] = (double) leftDownSelf / BOARD_SIZE;
    snake->network->layerIn[14] = (double) rightUpSelf / BOARD_SIZE;
    snake->network->layerIn[15] = (double) rightDownSelf / BOARD_SIZE;

    snake->network->layerIn[20] = (double) leftUpFruit / BOARD_SIZE;
    snake->network->layerIn[21] = (double) leftDownFruit / BOARD_SIZE;
    snake->network->layerIn[22] = (double) rightUpFruit / BOARD_SIZE;
    snake->network->layerIn[23] = (double) rightDownFruit / BOARD_SIZE;
}









