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

Snake *getSnake(int x, int y, SegmentDirection direction)
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
//                if (snake->tail->y < 0)
//                    snake->tail->y = BOARD_SIZE - 1;
                snake->tail->x = snake->head->x;
                break;
            case DOWN:
                snake->tail->y = snake->head->y + 1;
//                if (snake->tail->y >= BOARD_SIZE)
//                    snake->tail->y = 0;
                snake->tail->x = snake->head->x;
                break;
            case LEFT:
                snake->tail->y = snake->head->y;
                snake->tail->x = snake->head->x - 1;
//                if (snake->tail->x < 0)
//                    snake->tail->x = BOARD_SIZE - 1;
                break;
            case RIGHT:
                snake->tail->y = snake->head->y;
                snake->tail->x = snake->head->x + 1;
//                if (snake->tail->x >= BOARD_SIZE)
//                    snake->tail->x = 0;
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

void calculateDistances(Game *game, Snake *snake, Network *network)
{
    //wall distance straight
    network->layerIn[0] = sigmoid(snake->head->y);
    network->layerIn[1] = sigmoid(snake->head->x);
    network->layerIn[2] = sigmoid(BOARD_SIZE - snake->head->x);
    network->layerIn[3] = sigmoid(BOARD_SIZE - snake->head->y);

    //wall distance diagonal
    int tempX = snake->head->x;
    int tempY = snake->head->y;
    int leftUpDistance = 0;
    while(tempX-- && tempY--)
        leftUpDistance++;

    tempX = snake->head->x;
    tempY = snake->head->y;
    int leftDownDistance = 0;
    while(tempX-- && tempY++ <= BOARD_SIZE)
        leftDownDistance++;

    tempX = snake->head->x;
    tempY = snake->head->y;
    int rightDownDistance = 0;
    while(tempX++ <= BOARD_SIZE && tempY++ <= BOARD_SIZE)
        rightDownDistance++;

    tempX = snake->head->x;
    tempY = snake->head->y;
    int rightUpDistance = 0;
    while(tempX++ <= BOARD_SIZE && tempY--)
        rightUpDistance++;

    network->layerIn[4] = sigmoid(leftUpDistance);
    network->layerIn[5] = sigmoid(leftDownDistance);
    network->layerIn[6] = sigmoid(rightUpDistance);
    network->layerIn[7] = sigmoid(rightDownDistance);

    //self distance straight


    int leftSelf = BOARD_SIZE;
    int rightSelf = BOARD_SIZE;
    int upSelf = BOARD_SIZE;
    int downSelf = BOARD_SIZE;

    tempX = snake->head->x - 1;
    tempY = snake->head->y;
    int cnt = 0;

    while(tempX >= 0)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            leftSelf = cnt;
            break;
        }
        cnt++;
        tempX--;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y;
    cnt = 0;

    while(tempX <= BOARD_SIZE)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            rightSelf = cnt;
            break;
        }
        cnt++;
        tempX++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y + 1;
    cnt = 0;

    while(tempY <= BOARD_SIZE)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            downSelf = cnt;
            break;
        }
        cnt++;
        tempY++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y - 1;
    cnt = 0;

    while(tempY >= 0)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            upSelf = cnt;
            break;
        }
        cnt++;
        tempY--;
    }

    network->layerIn[8] = sigmoid(upSelf);
    network->layerIn[9] = sigmoid(downSelf);
    network->layerIn[10] = sigmoid(leftSelf);
    network->layerIn[11] = sigmoid(rightSelf);

    //self distance diagonal

    leftUpDistance = BOARD_SIZE;
    leftDownDistance = BOARD_SIZE;
    rightUpDistance = BOARD_SIZE;
    rightDownDistance = BOARD_SIZE;

    tempX = snake->head->x - 1;
    tempY = snake->head->y - 1;
    cnt = 0;

    while(tempY >= 0 && tempX >= 0)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            leftUpDistance = cnt;
            break;
        }
        cnt++;
        tempY--;
        tempX--;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y + 1;
    cnt = 0;

    while(tempY <= BOARD_SIZE && tempX <= BOARD_SIZE)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            rightDownDistance = cnt;
            break;
        }
        cnt++;
        tempY++;
        tempX++;
    }

    tempX = snake->head->x - 1;
    tempY = snake->head->y + 1;
    cnt = 0;

    while(tempY <= BOARD_SIZE && tempX >= 0)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            leftDownDistance = cnt;
            break;
        }
        cnt++;
        tempY++;
        tempX--;
    }

    tempX = snake->head->x + 1;
    tempY = snake->head->y - 1;
    cnt = 0;

    while(tempY >= 0 && tempX <= BOARD_SIZE)
    {
        if(isSnakeSegmentHere(snake, tempX, tempY))
        {
            rightUpDistance = cnt;
            break;
        }
        cnt++;
        tempY--;
        tempX++;
    }

    network->layerIn[12] = sigmoid(leftUpDistance);
    network->layerIn[13] = sigmoid(leftDownDistance);
    network->layerIn[14] = sigmoid(rightUpDistance);
    network->layerIn[15] = sigmoid(rightDownDistance);



    //fruit distance straight


    int leftFruit = BOARD_SIZE;
    int rightFruit = BOARD_SIZE;
    int upFruit = BOARD_SIZE;
    int downFruit = BOARD_SIZE;

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempX >= 0)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            leftFruit = cnt;
            break;
        }
        cnt++;
        tempX--;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempX <= BOARD_SIZE)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            rightFruit = cnt;
            break;
        }
        cnt++;
        tempX++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY <= BOARD_SIZE)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            downFruit = cnt;
            break;
        }
        cnt++;
        tempY++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY >= 0)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            upFruit = cnt;
            break;
        }
        cnt++;
        tempY--;
    }

    network->layerIn[16] = sigmoid(upFruit);
    network->layerIn[17] = sigmoid(downFruit);
    network->layerIn[18] = sigmoid(leftFruit);
    network->layerIn[19] = sigmoid(rightFruit);

    //fruit distance diagonal

    leftUpDistance = BOARD_SIZE;
    leftDownDistance = BOARD_SIZE;
    rightUpDistance = BOARD_SIZE;
    rightDownDistance = BOARD_SIZE;

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY >= 0 && tempX >= 0)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            leftUpDistance = cnt;
            break;
        }
        cnt++;
        tempY--;
        tempX--;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY <= BOARD_SIZE && tempX <= BOARD_SIZE)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            rightDownDistance = cnt;
            break;
        }
        cnt++;
        tempY++;
        tempX++;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY <= BOARD_SIZE && tempX >= 0)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            leftDownDistance = cnt;
            break;
        }
        cnt++;
        tempY++;
        tempX--;
    }

    tempX = snake->head->x;
    tempY = snake->head->y;
    cnt = 0;

    while(tempY >= 0 && tempX <= BOARD_SIZE)
    {
        if(isFruitHere(game, tempX, tempY))
        {
            rightUpDistance = cnt;
            break;
        }
        cnt++;
        tempY--;
        tempX++;
    }

    network->layerIn[20] = sigmoid(leftUpDistance);
    network->layerIn[21] = sigmoid(leftDownDistance);
    network->layerIn[22] = sigmoid(rightUpDistance);
    network->layerIn[23] = sigmoid(rightDownDistance);

}

bool isSnakeSegmentHere(Snake *snake, int x, int y)
{
    SnakeSegment *segment = snake->head;
    while (segment)
    {
        if(segment->x == x && segment->y == y)
            return true;
        segment = segment->next;
    }
    return false;
}









