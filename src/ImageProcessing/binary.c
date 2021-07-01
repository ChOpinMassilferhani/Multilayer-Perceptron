# include <err.h>
# include <math.h>
# include "SDL/SDL.h"
# include "SDL/SDL_image.h"
# include "pixel_operations.h"
# include "binary.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1, "Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

void to_grey( SDL_Surface *image_surface){
    int height = image_surface->h;
    int width = image_surface->w;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
            pixel = SDL_MapRGB(image_surface->format, average, average, average);
            put_pixel(image_surface, i, j, pixel);
        }
    }
}

int otsu(int histo[], SDL_Surface* img)
{
    double* seuil = calloc(256,sizeof(double));
    int len = img->w * img->h;
    for(int i = 0; i < 256; i++)
    {
        double first = 0.0;
        double second = 0.0;
        for(int j = 0; j < i;j++)
        {
            double weight = 0.0;
            double mean = 0.0;
            double var = 0.0;
            int k = 0;
            while(k<i)
            {
                mean += k*histo[k];
                weight += histo[k];
                k++;
            }
            weight = weight / (double) len;
            mean = mean / (double) len;
            k = 0;
            while(k < j)
            {
                var += pow(k - fabs(mean),2) * histo[k];
                k++;
            }
            var = var / (double) len;
            first = weight * var;
        }
        for(int j = i; j < 256;j++)
        {
            double weight = 0.0;
            double mean = 0.0;
            double var = 0.0;
            int k = i;
            while(k<256)
            {
                mean += k*histo[k];
                weight += histo[k];
                k++;
            }
            weight = weight / (double) len;
            mean = mean / (double) len;
            k = 0;
            while(k < 256)
            {
                var += pow(k - fabs(mean),2) * histo[k];
                k++;
            }
            var = var / (double) len;
            second = weight * var;
        }
        seuil[i] = first + second;
    }
    int index = 0;
    for(int i = 0; i < 256; i++)
    {
        if(seuil[i] < seuil[index])
            index = i;
    }
    free(seuil);
    return index;
}

void create_histo2(int array[], SDL_Surface* img) //Histogram w/near neighbors
{
    int width = img->w;
    int height = img->h;
    int grey;

    Uint8 r,g,b;


    for(int i=0; i < height; i++)
    {
        for(int j=0; j < width; j++)
        {
            Uint32 pixel1 = get_pixel(img,j,i);
            SDL_GetRGB(pixel1, img->format, &r, &g, &b);
            grey = ((int) r + (int) g + (int) b) / 3;
            array[grey] += 1;
        }
    }

}

void Binarisation(SDL_Surface* img, int seuil) //Binarization
{
    int width = img->w;
    int height = img->h;



    Uint32 pixel;
    Uint32 newpixel;

    for(int i=0; i<height;i++)
    {
        for(int j=0; j<width; j++)
        {
            pixel = get_pixel(img,j,i);

            Uint8 r,g,b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);


            if(r<= seuil)
            {
                newpixel = SDL_MapRGB(img->format, 0,0,0);
                put_pixel(img,j,i,newpixel);
            }

            else
            {
                newpixel = SDL_MapRGB(img->format,255,255,255);
                put_pixel(img,j,i,newpixel);
            }
        }
    }

}


SDL_Surface *display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE | SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while (event.type != SDL_KEYUP);
}

void binary(SDL_Surface *image_surface){

    to_grey(image_surface);

    int* Histo = calloc(256,sizeof(int));
    create_histo2(Histo,image_surface);
    int t = otsu(Histo,image_surface);
    Binarisation(image_surface,t);
    free(Histo);
}
