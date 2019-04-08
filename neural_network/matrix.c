//
// Created by filip on 4/8/19.
//

#include <stdlib.h>
#include "matrix.h"

void initMatrix(Matrix *matrix, int rows, int columns)
{
    matrix->elements = malloc(rows * columns * sizeof(double));
    matrix->rows = rows;
    matrix->columns = columns;
}

void randomizeMatrix(Matrix *m)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->columns; j++)
        {
            setValue(m, i, j, (rand() / (double) RAND_MAX) * 2 - 1);
        }
    }
}

void dotProductMatrices(Matrix *m1, Matrix *m2, Matrix *result)
{
    if (result->elements)
        free(result->elements);
    initMatrix(result, m1->rows, m2->columns);
    double element;
    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m2->columns; j++)
        {
            element = 0;
            for (int k = 0; k < m2->rows; k++)
                element += getValue(m2, k, j) * getValue(m1, i, k);
            setValue(result, i, j, element);
        }
    }
}

void setValue(Matrix *matrix, int row, int column, double value)
{
    matrix->elements[column + row * matrix->columns] = value;

}

double getValue(Matrix *matrix, int row, int column)
{
    return matrix->elements[column + row * matrix->columns];
}



void destroyMatrix(Matrix *matrix)
{
    free(matrix->elements);
    matrix->elements = NULL;
}

void cloneMatrix(Matrix *parent, Matrix *child)
{
    for (int i = 0; i < parent->rows; i++)
    {
        for (int j = 0; j < parent->columns; ++j)
        {
            setValue(child, i, j, getValue(parent, i, j));
        }
    }
}

void crossoverMatrix(Matrix *parent1, Matrix *parent2, Matrix *child)
{
    int rand1 = rand() % parent1->rows;
    int rand2 = rand() % parent1->columns;

    for (int i = 0; i < parent1->rows; ++i)
    {
        for (int j = 0; j < parent1->columns; ++j)
        {
            if (i < rand1 || (i == rand1 && j <= rand2))
                setValue(child, i, j, getValue(parent1, i, j));
            else
                setValue(child, i, j, getValue(parent2, i, j));
        }
    }
}

void mutateMatrix(double rate, Matrix *matrix)
{
    for (int i = 0; i < matrix->rows; ++i)
    {
        for (int j = 0; j < matrix->columns; ++j)
        {
            if ((rand() / (double) RAND_MAX) <= rate)
            {
                double change = (rand() / (double) RAND_MAX) / 4.0 - 0.125;
                setValue(matrix, i, j, getValue(matrix, i, j) + change);
                if (getValue(matrix, i, j) < -1)
                    setValue(matrix, i, j, -1);
                if (getValue(matrix, i, j) > 1)
                    setValue(matrix, i, j, 1);
            }

        }
    }
}


