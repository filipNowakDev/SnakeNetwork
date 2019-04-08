//
// Created by filip on 4/5/19.
//

#ifndef SNAKENETWORK_NETWORK_H
#define SNAKENETWORK_NETWORK_H


#include "matrix.h"

#define INPUT_NEURONS 24
#define HIDDEN_NEURONS 16
#define OUTPUT_NEURONS 4

typedef enum SegmentDirection SegmentDirection;

typedef struct Network
{
    Matrix layerIn;
    Matrix layerHidden1;
    Matrix layerHidden2;
    Matrix layerOut;

    Matrix weightsInHidden;
    Matrix weightsHiddenHidden;
    Matrix weightsHiddenOut;


} Network;

void randomizeNetwork(Network *network);

double sigmoid(double x);

void calculateOutput(Network *network);

SegmentDirection getOutputDirection(Network *network);

void cloneNetwork(Network *parent, Network *child);

void crossoverNetworks(Network *parent1, Network *parent2, Network *child);

void mutateNetwork(double rate, Network *network);

void destroyNetwork(Network *network);

void activateLayer(Matrix *matrix);

void initNetwork(Network *network);


#endif //SNAKENETWORK_NETWORK_H
