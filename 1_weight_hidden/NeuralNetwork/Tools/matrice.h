#ifndef MATRICE_H_
#define MATRICE_H_

# include <stdio.h>
# include <time.h>

struct matrice
{
    double *mat;
    int height;
    int width;
};

struct matrice *matrice_init(int height, int width);

struct matrice *transpose( struct matrice *cur);

void add(struct matrice *r, struct matrice *m1, struct matrice *m2);

void mult(struct matrice *r, struct matrice *m1, struct matrice *m2);

void randomise_mat(struct matrice *cur);

void print_matrix(struct matrice *cur);


#endif