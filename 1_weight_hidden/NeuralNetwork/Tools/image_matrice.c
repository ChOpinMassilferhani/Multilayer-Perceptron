#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Network/save.h"
#include "../../ImageProcessing/binary.h"
#include "image_matrice.h"

int canevas = 40;

double **generate_matrices(char *path)
{
    char *tlen = calloc(60, sizeof(char));
    
    sprintf(tlen, "%s/Len.txt",path);
    FILE *file;
    file =   fopen(tlen, "r");
    size_t len;
    
    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d",strerror(errno),errno);

    if (getline(&tlen, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    
    int len_data = atoi(tlen);

    if(len_data != 67)
        errx(EXIT_FAILURE,"Not 67");

    double **data = calloc(len_data, sizeof(int *));

    for (int i = 0; i < len_data; i++)
    {
        sprintf(tlen, "%s/%d.bmp", path, i);
        SDL_Surface *image_surface = load_image(tlen);
        data[i] = img_to_mat(image_surface);
    }
    free(tlen);

    return data;
}

double *img_to_mat(SDL_Surface *image_surface)
{
    double *cur = calloc(canevas * canevas, sizeof(double));
    int hight = image_surface->h;
    int weight = image_surface->w;

    int mid_init = ((canevas - weight) / 2) + ((canevas - hight) / 2) * canevas;
    Uint8 r, g, b;

    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < weight; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            cur[mid_init + canevas * i + j] = (r == 0) ? 1 : 0;
        }
    }

    return cur;
}
