#ifndef SAVE_LETTER_H_
#define SAVE_LETTER_H_

void save_letter(SDL_Surface *image_surface);

SDL_Surface *Cut(SDL_Surface *img, int x, int y, int w, int h);

int getHeight(SDL_Surface *image_surface, int i, int j);

int getWidth(SDL_Surface *image_surface, int i, int j);

void save_char(SDL_Surface *image_surface, int w, int h, int Line, int Word, int Char);

#endif