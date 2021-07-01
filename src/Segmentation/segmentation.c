#include <err.h>
#include <stdlib.h>
#include "binary.h"
#include "pixel_operations.h"
#include "save_letter.h"
#include "segmentation.h"

void print_vertical(SDL_Surface *image_surface, int i, int j, int k)
{
    Uint32 red = SDL_MapRGB(image_surface->format, 255, 0, 0);
    
    while (j-1 <= k)
    {
        Uint32 pixel = get_pixel(image_surface, i, k);
        Uint8 r, g, b;
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        put_pixel(image_surface, i, k, red);
        k--;
    }
}

void vertical_cutting(SDL_Surface *image_surface)
{
    int height = image_surface->h;
    int width = image_surface->w;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint8 r, g, b;
            do
            {
                Uint32 pixel = get_pixel(image_surface, i, j);
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                j++;
            } while (r == 255 && g == 0 && j < height);

            int k = j;
            do
            {
                
                Uint32 pixel = get_pixel(image_surface, i, k);
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                k++;
            } while (r == 255 && g == 255 && k < height);

            if (r == 255 && g == 0){
                print_vertical(image_surface, i, j, k);
            }
            else{
                do
                {
                    Uint32 pixel = get_pixel(image_surface, i, j);
                    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                    j++;
                } while (!(r == 255 && g == 0) && j < height);
            }
        }
    }
}

void print_horizontal(SDL_Surface *image_surface, int index)
{
    int width = image_surface->w;
    for (int i = 0; i < width; i++)
    {
        Uint32 pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, i, index, pixel);
    }
}

void horizontal_cutting(SDL_Surface *image_surface)
{
    int height = image_surface->h;
    int width = image_surface->w;

    for (int i = 0; i < height; i++)
    {
        int j = 0;
        for (; j < width; j++)
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if (r == 0)
                break;
        }
        if ( j == width)
            print_horizontal(image_surface, i);
    }
}




void segmentation(SDL_Surface *image_surface)
{
    //SDL_Surface *screen_surface = display_image(image_surface);

    //update_surface(screen_surface, image_surface);

    binary(image_surface);

    horizontal_cutting(image_surface);  

    vertical_cutting(image_surface);

    //update_surface(screen_surface, image_surface);

    //wait_for_keypressed();

    //update_surface(screen_surface, image_surface);

    //wait_for_keypressed();

    //SDL_FreeSurface(screen_surface);

    save_letter(image_surface);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./segmentation <path image>");

    
    SDL_Surface *image_surface = load_image(argv[1]);

    segmentation(image_surface);

    SDL_FreeSurface(image_surface);
}
