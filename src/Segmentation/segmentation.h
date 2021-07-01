#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include "binary.h"

void print_horizontal(SDL_Surface *image_surface, int index);

void horizontal_cutting(SDL_Surface *image_surface);

void print_vertical(SDL_Surface *image_surface, int index_i, int index_j, int k);

void vertical_cutting(SDL_Surface *image_surface);

void segmentation(SDL_Surface *image_surface);

#endif
