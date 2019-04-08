//
// Created by filip on 4/5/19.
//

#include "network.h"
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include "../snake/snake.h"



void randomizeNetwork(Network *network)
{
    randomizeMatrix(&network->weightsInHidden);
    randomizeMatrix(&network->weightsHiddenHidden);
    randomizeMatrix(&network->weightsHiddenOut);
}

void initNetwork(Network *network)
{
    initMatrix(&network->layerIn, INPUT_NEURONS + 1, 1);
    initMatrix(&network->layerHidden1, HIDDEN_NEURONS + 1, 1);
    initMatrix(&network->layerHidden2, HIDDEN_NEURONS + 1, 1);
    initMatrix(&network->layerOut, OUTPUT_NEURONS + 1, 1);

    initMatrix(&network->weightsInHidden, HIDDEN_NEURONS, INPUT_NEURONS + 1);
    initMatrix(&network->weightsHiddenHidden, HIDDEN_NEURONS, HIDDEN_NEURONS + 1);
    initMatrix(&network->weightsHiddenOut, OUTPUT_NEURONS, HIDDEN_NEURONS + 1);
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

void calculateOutput(Network *network)
{

    setValue(&network->layerIn, network->layerIn.rows - 1, 0, 1);

    dotProductMatrices(&network->weightsInHidden, &network->layerIn, &network->layerHidden1);
    activateLayer(&network->layerHidden1);
    setValue(&network->layerHidden1, network->layerHidden1.rows - 1, 0, 1);

    dotProductMatrices(&network->weightsHiddenHidden, &network->layerHidden1, &network->layerHidden2);
    activateLayer(&network->layerHidden2);
    setValue(&network->layerHidden2, network->layerHidden2.rows - 1, 0, 1);

    dotProductMatrices(&network->weightsHiddenOut, &network->layerHidden2, &network->layerOut);
    activateLayer(&network->layerOut);


}

void activateLayer(Matrix *matrix)
{
    for(int i = 0; i < matrix->rows; i++)
        setValue(matrix, i, 0, sigmoid(getValue(matrix, i, 0)));
}

SegmentDirection getOutputDirection(Network *network)
{
    double max = 0;
    SegmentDirection dir = NONE;

    for (int i = 0; i < OUTPUT_NEURONS; ++i)
    {
        if (getValue(&network->layerOut, i, 0) > max)
        {
            max = getValue(&network->layerOut, i, 1);
            dir = i;
        }
    }
    return dir;
}

void cloneNetwork(Network *parent, Network *child)
{
    cloneMatrix(&parent->weightsInHidden, &child->weightsInHidden);
    cloneMatrix(&parent->weightsHiddenHidden, &child->weightsHiddenHidden);
    cloneMatrix(&parent->weightsHiddenOut, &child->weightsHiddenOut);
}


void crossoverNetworks(Network *parent1, Network *parent2, Network *child)
{
    crossoverMatrix(&parent1->weightsInHidden, &parent2->weightsInHidden, &child->weightsInHidden);
    crossoverMatrix(&parent1->weightsHiddenOut, &parent2->weightsHiddenOut, &child->weightsHiddenOut);
    crossoverMatrix(&parent1->weightsHiddenHidden, &parent2->weightsHiddenHidden, &child->weightsHiddenHidden);
}

void mutateNetwork(double rate, Network *network)
{
    mutateMatrix(rate, &network->weightsInHidden);
    mutateMatrix(rate, &network->weightsHiddenHidden);
    mutateMatrix(rate, &network->weightsHiddenOut);
}

void destroyNetwork(Network *network)
{
    destroyMatrix(&network->layerIn);
    destroyMatrix(&network->layerOut);
    destroyMatrix(&network->layerHidden1);
    destroyMatrix(&network->layerHidden2);
    destroyMatrix(&network->weightsInHidden);
    destroyMatrix(&network->weightsHiddenHidden);
    destroyMatrix(&network->weightsHiddenOut);
}


