# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "matrice.h"



struct matrice *matrice_init(int height, int width)
{
    struct matrice *tmp = calloc(1,sizeof(struct matrice));
    tmp->height = height;
    tmp->width = width;
    tmp->mat = calloc( height * width , sizeof(double) );
    return tmp;
}

void add(struct matrice *r, struct matrice *m1, struct matrice *m2)
{
    int len = m1->width * m1->height;
    for (int i = 0; i < len; i++)
    {
        r->mat[i] = m1->mat[i] + m2->mat[i];
    }
}

void mult(struct matrice *r, struct matrice *m1, struct matrice *m2)
{
    int r1 = m1->height;
    int c1 = m1->width;
    int c2 = m2->width;
    for (int i = 0; i < r1; i++)
    {
        for (int j = 0; j < c2; j++)
        {
            double sum = 0;
            for (int k = 0; k < c1; k++)
                sum = sum + m1->mat[i * c1 + k] * m2->mat[k * c2 + j];
            r->mat[i * c2 + j] = sum;
        }
    }
}

void randomise_mat(struct matrice *cur)
{
    int len = cur->width * cur->height;
    for (int i = 0; i < len; i++)
    {
        cur->mat[i] = (((double)rand())/(double)RAND_MAX)*2 -1;
    }
    
}

void print_matrix(struct matrice *cur)
{
    size_t rows = cur->height;
    size_t cols = cur->width;



    for(size_t i = 0; i < rows;i++)
    {
        for(size_t j = 0; j < cols;j++)
        {
            printf("%lf ",cur->mat[i*cols + j]);
        }
        printf("\n");
    }
}