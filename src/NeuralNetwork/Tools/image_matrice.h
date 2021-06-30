#ifndef IM_TO_MAT_H_
#define IM_TO_MAT_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

double **generate_matrices( char *path);

double *img_to_mat(SDL_Surface *image_surface);

#endif
