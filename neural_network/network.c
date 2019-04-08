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

    for(int i = 0; i < INPUT_NEURONS; i++)
    {
        for(int j = 0; j < HIDDEN_NEURONS; j++)
        {
            network->weightsIn[i][j] = (rand()/(double)RAND_MAX)*4 - 2;
            network->biasIn[i][j] = (rand()/(double)RAND_MAX)*4 - 2;
        }
    }

    for(int i = 0; i < HIDDEN_NEURONS; i++)
    {
        for(int j = 0 ; j < OUTPUT_NEURONS; j++)
        {
            network->weightsOut[i][j] = (rand()/(double)RAND_MAX)*4 - 2;
            network->biasOut[i][j] = (rand()/(double)RAND_MAX)*4 - 2;
        }
    }
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

void calculateOutput(Network* network)
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
        if(network->layerOut[i] > max)
        {
            max = network->layerOut[i];
            dir = i;
        }
    }
    return dir;
}


