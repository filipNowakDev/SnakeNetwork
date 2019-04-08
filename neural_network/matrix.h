//
// Created by filip on 4/8/19.
//

#ifndef SNAKENETWORK_MATRIX_H
#define SNAKENETWORK_MATRIX_H


typedef struct Matrix
{
    double * elements;
    int rows;
    int columns;
}Matrix;

void initMatrix(Matrix* matrix, int rows, int columns);

void randomizeMatrix(Matrix* m);

void setValue(Matrix* matrix, int row, int column, double value);

double getValue(Matrix* matrix, int row, int column);

void dotProductMatrices(Matrix* m1, Matrix* m2, Matrix* result);

void cloneMatrix(Matrix* parent, Matrix* child);

void crossoverMatrix(Matrix* parent1, Matrix* parent2, Matrix* child);

void mutateMatrix(double rate, Matrix* matrix);

void destroyMatrix(Matrix* matrix);



#endif //SNAKENETWORK_MATRIX_H
