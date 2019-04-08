//
// Created by filip on 4/5/19.
//

#include "network.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "../snake/snake.h"

void initRandomNetwork(Network *network)
{

    for (int i = 0; i < INPUT_NEURONS; i++)
    {
        for (int j = 0; j < HIDDEN_NEURONS; j++)
        {
            network->weightsIn[i][j] = (rand() / (double) RAND_MAX) * 2 - 1;
            network->biasIn[i][j] = (rand() / (double) RAND_MAX) * 2 - 1;
        }
    }

    for (int i = 0; i < HIDDEN_NEURONS; i++)
    {
        for (int j = 0; j < OUTPUT_NEURONS; j++)
        {
            network->weightsOut[i][j] = (rand() / (double) RAND_MAX) * 2 - 1;
            network->biasOut[i][j] = (rand() / (double) RAND_MAX) * 2 - 1;
        }
    }
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

void calculateOutput(Network *network)
{
    for (int j = 0; j < HIDDEN_NEURONS; ++j)
        network->layerHidden[j] = 0;

    for (int i = 0; i < INPUT_NEURONS; ++i)
        for (int j = 0; j < HIDDEN_NEURONS; ++j)
            network->layerHidden[j] += network->layerIn[i] * network->weightsIn[i][j] + network->biasIn[i][j];

    for (int j = 0; j < HIDDEN_NEURONS; ++j)
        network->layerHidden[j] = sigmoid(network->layerHidden[j]);


    for (int j = 0; j < OUTPUT_NEURONS; ++j)
        network->layerOut[j] = 0;

    for (int i = 0; i < HIDDEN_NEURONS; ++i)
        for (int j = 0; j < OUTPUT_NEURONS; ++j)
            network->layerOut[j] += network->layerHidden[i] * network->weightsOut[i][j] + network->biasOut[i][j];

    for (int j = 0; j < HIDDEN_NEURONS; ++j)
        network->layerOut[j] = sigmoid(network->layerOut[j]);
}

SegmentDirection getOutputDirection(Network *network)
{
    double max = 0;
    SegmentDirection dir = NONE;

    for (int i = 0; i < OUTPUT_NEURONS; ++i)
    {
        if (network->layerOut[i] > max)
        {
            max = network->layerOut[i];
            dir = i;
        }
    }
    return dir;
}

void cloneNetwork(Network *parent, Network *child)
{
    for (int i = 0; i < INPUT_NEURONS; i++)
    {
        for (int j = 0; j < HIDDEN_NEURONS; j++)
        {
            child->weightsIn[i][j] = parent->weightsIn[i][j];
            child->biasIn[i][j] = parent->biasIn[i][j];
        }
    }

    for (int i = 0; i < HIDDEN_NEURONS; i++)
    {
        for (int j = 0; j < OUTPUT_NEURONS; j++)
        {
            child->weightsOut[i][j] = parent->weightsOut[i][j];
            child->biasOut[i][j] = parent->biasOut[i][j];
        }
    }
}


void crossoverNetworks(Network *parent1, Network *parent2, Network *child)
{
    int rand1 = rand() % INPUT_NEURONS;
    int rand2 = rand() % HIDDEN_NEURONS;

    for (int i = 0; i < INPUT_NEURONS; ++i)
    {
        for (int j = 0; j < OUTPUT_NEURONS; ++j)
        {
            if (i < rand1 || (i == rand1 && j <= rand2))
            {
                child->weightsIn[i][j] = parent1->weightsIn[i][j];
                child->biasIn[i][j] = parent1->biasIn[i][j];
            } else
            {
                child->weightsIn[i][j] = parent2->weightsIn[i][j];
                child->biasIn[i][j] = parent2->biasIn[i][j];
            }
        }
    }

    rand1 = rand() % HIDDEN_NEURONS;
    rand2 = rand() % OUTPUT_NEURONS;

    for (int i = 0; i < HIDDEN_NEURONS; ++i)
    {
        for (int j = 0; j < OUTPUT_NEURONS; ++j)
        {
            if (i < rand1 || (i == rand1 && j <= rand2))
            {
                child->weightsOut[i][j] = parent1->weightsOut[i][j];
                child->biasOut[i][j] = parent1->biasOut[i][j];
            } else
            {
                child->weightsOut[i][j] = parent2->weightsOut[i][j];
                child->biasOut[i][j] = parent2->biasOut[i][j];
            }
        }
    }
}

void mutateNetwork(double rate, Network *network)
{
    for (int i = 0; i < INPUT_NEURONS; ++i)
    {
        for (int j = 0; j < OUTPUT_NEURONS; ++j)
        {
            if (rand() / (double) RAND_MAX <= rate)
            {
                network->weightsIn[i][j] += (rand() / (double) RAND_MAX) / 5.0  - 0.1;
                if(network->weightsIn[i][j] < -1)
                    network->weightsIn[i][j] = -1;
                if(network->weightsIn[i][j] > 1)
                    network->weightsIn[i][j] = 1;
            }
            if (rand() / (double) RAND_MAX <= rate)
            {
                network->biasIn[i][j] += (rand() / (double) RAND_MAX) / 5.0 - 0.1;
                if(network->biasIn[i][j] < -1)
                    network->biasIn[i][j] = -1;
                if(network->biasIn[i][j] > 1)
                    network->biasIn[i][j] = 1;
            }

        }
    }

    for (int i = 0; i < HIDDEN_NEURONS; ++i)
    {
        for (int j = 0; j < OUTPUT_NEURONS; ++j)
        {
            if (rand() / (double) RAND_MAX <= rate)
            {
                network->weightsOut[i][j] += (rand() / (double) RAND_MAX) / 5.0 - 0.1;
                if(network->weightsOut[i][j] < -1)
                    network->weightsOut[i][j] = -1;
                if(network->weightsOut[i][j] > 1)
                    network->weightsOut[i][j] = 1;
            }
            if (rand() / (double) RAND_MAX <= rate)
            {
                network->biasOut[i][j] += (rand() / (double) RAND_MAX) / 5.0 - 0.1;
                if(network->biasOut[i][j] < -1)
                    network->biasOut[i][j] = -1;
                if(network->biasOut[i][j] > 1)
                    network->biasOut[i][j] = 1;
            }
        }
    }
}


