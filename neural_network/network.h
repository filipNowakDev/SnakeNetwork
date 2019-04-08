//
// Created by filip on 4/5/19.
//

#ifndef SNAKENETWORK_NETWORK_H
#define SNAKENETWORK_NETWORK_H


#define INPUT_NEURONS 24
#define HIDDEN_NEURONS 16
#define OUTPUT_NEURONS 4

typedef enum SegmentDirection SegmentDirection;

typedef struct Network
{
    double layerIn[INPUT_NEURONS];
    double layerHidden[HIDDEN_NEURONS];
    double layerOut[OUTPUT_NEURONS];
    double weightsIn[INPUT_NEURONS][HIDDEN_NEURONS];
    double biasIn[INPUT_NEURONS][HIDDEN_NEURONS];
    double weightsOut[HIDDEN_NEURONS][OUTPUT_NEURONS];
    double biasOut[HIDDEN_NEURONS][OUTPUT_NEURONS];
} Network;

void initRandomNetwork(Network *network);

double sigmoid(double x);

void calculateOutput(Network *network);

SegmentDirection getOutputDirection(Network *network);

void cloneNetwork(Network *parent, Network *child);

void crossoverNetworks(Network *parent1, Network *parent2, Network *child);

void mutateNetwork(double rate, Network *network);


#endif //SNAKENETWORK_NETWORK_H
