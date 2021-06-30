#ifndef BINARY_H_
#define BINARY_H_


void binary(SDL_Surface *image_surface);

void init_sdl();

SDL_Surface *load_image(char *path);

void to_grey( SDL_Surface *image_surface);

int otsu(int histo[], SDL_Surface* img);

void create_histo2(int array[], SDL_Surface* img);

void Binarisation(SDL_Surface* img, int seuil);

SDL_Surface *display_image(SDL_Surface *img);

void wait_for_keypressed();

#endif