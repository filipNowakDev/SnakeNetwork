//
// Created by filip on 4/8/19.
//

#include "population.h"
#include "../core/game.h"


void setBestGlobalSnake(Population *p);

void initPopulation(Population *p)
{
    p->snakes = malloc(POPULATION_SIZE * sizeof(Snake));
    p->generation = 0;
    p->populationBestSnake = 0;
    p->currentBestSnake = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        initSnake(p->snakes + i, BOARD_SIZE / 2, BOARD_SIZE / 2, DOWN);
        randomizeNetwork(&p->snakes[i].network);
    }
}

void updatePopulation(Population *population)
{
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        Fruit *fruit = &population->snakes[i].fruit;
        Snake *snake = population->snakes + i;
        if (snake->isAlive)
        {
            calculateDistances(fruit, snake);
            calculateOutput(&snake->network);
            setSnakeDirection(snake, getOutputDirection(&snake->network));

            if (updateFruit(fruit, population->snakes + i))
                addSnakeSegment(snake);
            updateSnakePosition(snake);

            if (!isSnakeValid(snake))
                snake->isAlive = false;
        }
    }

    setBestSnake(population);
}

void setBestSnake(Population *p)
{
    if (areAnySnakesAlive(p))
    {
        int maxLength = 0;
        int maxIndex = 0;
        for (int i = 0; i < POPULATION_SIZE; ++i)
        {
            if (p->snakes[i].isAlive && p->snakes[i].length > maxLength)
            {
                maxLength = p->snakes[i].length;
                maxIndex = 0;
            }
        }
        if (maxLength > p->snakes[p->currentBestSnake].length)
            p->currentBestSnake = maxIndex;

        if (p->snakes[p->currentBestSnake].length > p->snakes[p->populationBestSnake].length)
            p->populationBestSnake = p->currentBestSnake;

        if (!p->snakes[p->currentBestSnake].isAlive || maxLength > p->snakes[p->currentBestSnake].length +5   ) {

            p->currentBestSnake  = maxIndex;
        }
    }
}

bool areAnySnakesAlive(Population *population)
{
    for (int i = 0; i < POPULATION_SIZE; ++i)
        if (population->snakes[i].isAlive)
            return true;
    return false;
}

void destroyPopulation(Population *population)
{
    for (int i = 0; i < POPULATION_SIZE; ++i)
        destroySnake(population->snakes + i);
    free(population->snakes);
}

void nextGeneration(Population *p)
{
    Snake *newSnakes = malloc(POPULATION_SIZE * sizeof(Snake));
    p->generation++;
    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        calculateFitness(p->snakes + i);
        initSnake(newSnakes + i, BOARD_SIZE / 2, BOARD_SIZE / 2, DOWN);
    }
    setBestGlobalSnake(p);

    cloneNetwork(&p->snakes[p->populationBestSnake].network, &newSnakes[0].network);

    for (int i = 1; i < 100; ++i)
    {
        cloneNetwork(&p->snakes[p->populationBestSnake].network, &newSnakes[i].network);
        mutateNetwork(MUTATION_RATE, &newSnakes[i].network);
    }

    for (int j = 100; j < POPULATION_SIZE; ++j)
    {
        Snake *parent1 = tournamentSelect(p);
        Snake *parent2 = tournamentSelect(p);

        crossoverNetworks(&parent1->network, &parent2->network, &newSnakes[1].network);
        mutateNetwork(MUTATION_RATE, &newSnakes[1].network);
    }

    destroyPopulation(p);
    p->snakes = newSnakes;
    p->currentBestSnake = 0;
    p->populationBestSnake = 0;
}

void setBestGlobalSnake(Population *p)
{
    long max = 0;
    int maxIndex = 0;
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        if (p->snakes[i].fitness > max)
        {
            max = p->snakes[i].fitness;
            maxIndex = i;
        }
    }
    if (max > p->populationBestFitness)
    {
        p->populationBestFitness = max;
        p->populationBestSnake = maxIndex;
    }
}


Snake *tournamentSelect(Population *p)
{
    const static int BEST_SELECTED = POPULATION_SIZE / 50;
    const static int RANDOMLY_SELECTED = POPULATION_SIZE / 50;
    Snake *randomlySelected[RANDOMLY_SELECTED];
    Snake *bestSelected[BEST_SELECTED];
    for (int i = 0; i < BEST_SELECTED; i++)
    {
        for (int j = 0; j < RANDOMLY_SELECTED; ++j)
        {
            randomlySelected[j] = &p->snakes[rand() % POPULATION_SIZE];
        }
        qsort(randomlySelected, RANDOMLY_SELECTED, sizeof(Snake *), compareSnakeFitness);
        bestSelected[i] = randomlySelected[0];
    }
    qsort(bestSelected, BEST_SELECTED, sizeof(Snake *), compareSnakeFitness);
    return bestSelected[0];
}

