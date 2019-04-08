//
// Created by filip on 4/8/19.
//

#ifndef SNAKENETWORK_POPULATION_H
#define SNAKENETWORK_POPULATION_H


#include "../snake/snake.h"

typedef struct Game Game;
#define POPULATION_SIZE 1000
#define MUTATION_RATE 0.90

typedef struct Population
{
    int generation;
    int currentBestSnake;
    int populationBestSnake;
    Snake *snakes;
    double populationBestFitness;
} Population;


void initPopulation(Population *p);

void updatePopulation(Population *population);

void setBestSnake(Population *p);

bool areAnySnakesAlive(Population *population);

void destroyPopulation(Population *population);

void nextGeneration(Population *p);

Snake *tournamentSelect(Population *p);

#endif //SNAKENETWORK_POPULATION_H
