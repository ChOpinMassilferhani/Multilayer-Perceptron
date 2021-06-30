# include <stdio.h>
# include <sys/stat.h>
# include "SDL/SDL.h"
# include "SDL/SDL_image.h"
# include "save_letter.h"
# include "../ImageProcessing/pixel_operations.h"
# include "../ImageProcessing/binary.h"


# define max(a,b)(a>b)?a:b

int max_w = 0;
int max_h = 0;

void ResetColor(SDL_Surface *img)
{
    int w = img->w;
    int h = img->h;
    Uint8 r, g, b;
    Uint32 pixel, new_pixel;

    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);

            if (((r == 255) && (g == 0) && (b == 0)) ||
                ((r == 0) && (g == 255) && (b == 0)) || ((r == 0) && (g == 0) && (b == 255)))
            {
                new_pixel = SDL_MapRGB(img->format,
                                       255, 255, 255);
                put_pixel(img, i, j, new_pixel);
            }
        }
    }
}

void save_word(SDL_Surface *image_surface, int Begin, int End, int i, int Line, int Word)
{
    int Char = 0;
    Begin--;
    char *str = calloc(60, sizeof(char));
    for (; Begin < End; Begin++)
    {
        Uint8 r, g, b;
        Uint32 pixel = get_pixel(image_surface, Begin, i);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        if (!(r == 255 && g == 0))
        {
            save_char(image_surface, Begin, i, Line, Word, Char);
            Char++;
        }
    }
    sprintf(str,"./Text/Line%d/Word%d/Len.txt",Line,Word);
    FILE *fp = fopen(str,"w");
    fprintf(fp,"%d",Char);
    fclose(fp);
    free(str);
}

void save_line(SDL_Surface *image_surface, int j, int i, int Line)
{
    int width = image_surface->w;
    int first = j;
    int Word = 0;
    char *str = calloc(60, sizeof(char));

    for (; j < width; j++)
    {
        Uint8 r, g, b;
        do
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            j++;
        } while (!(r == 255 && g == 0) && j < width);
        int count = 0;

        do
        {
            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            j++;
            count++;
        } while ((r == 255 && g == 0) && j < width);

        if (count > 8) //13
        {
            memset(str, 0, 60);
            sprintf(str, "./Text/Line%d/Word%d", Line, Word);
            mkdir(str, 0777);

            save_word(image_surface, first, j - count + 1, i, Line, Word);
            first = j;
            Word++;
        }
    }
    sprintf(str,"./Text/Line%d/Len.txt",Line);
    FILE *fp = fopen(str,"w");
    fprintf(fp,"%d",Word);
    fclose(fp);
    free(str);
}

void save_letter(SDL_Surface *image_surface)
{
    mkdir("./Text", 0777);

    int height = image_surface->h;
    int width = image_surface->w;

    char *str = calloc(60, sizeof(char));
    int Line = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint8 r, g, b;

            Uint32 pixel = get_pixel(image_surface, j, i);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if (!(r == 255 && g == 0))
            {
                memset(str, 0, 60);
                sprintf(str, "./Text/Line%d", Line);
                mkdir(str, 0777);

                save_line(image_surface, j, i, Line);
                Line++;
            }
        }
    }
    FILE *fp = fopen("./Text/Len.txt","w");
    fprintf(fp,"%d",Line);
    fclose(fp);
    free(str);
}

SDL_Surface *Cut(SDL_Surface *img, int x, int y, int w, int h)
{
    SDL_Rect CutZone = {x, y, w, h};
    SDL_Surface *NewImg = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_UnlockSurface(img);
    SDL_UnlockSurface(NewImg);

    SDL_BlitSurface(img, &CutZone, NewImg, NULL);
    ResetColor(NewImg);
    return NewImg;
}

SDL_Surface *getlim(SDL_Surface *charactere)
{

    int hight = charactere->h;
    int weight = charactere->w;

    int Begin = 0;
    int End = 0;

    Uint8 r, g, b;

    int breakk = 0;
    int i = 0;

    for (; i < hight; i++)
    {
        for (int j = 0; j < weight; j++)
        {
            Uint32 pixel = get_pixel(charactere, j, i);
            SDL_GetRGB(pixel, charactere->format, &r, &g, &b);
            if (r == 0)
            {
                Begin = i;
                breakk = 1;
                break;
            }
        }
        if (breakk)
            break;
    }

    breakk = 0;
    for (; i < hight; i++)
    {
        for (int j = 0; j < weight; j++)
        {
            breakk = 0;
            Uint32 pixel = get_pixel(charactere, j, i);
            SDL_GetRGB(pixel, charactere->format, &r, &g, &b);
            if (r == 0)
            {
                End = i;
                break;
            }
        }
    }

    End = (End == 0) ? hight - 1 : End; // if the full leter is already cuted
    return Cut(charactere, 0, Begin, weight, End - Begin);
}

void save_char(SDL_Surface *image_surface, int w, int h, int Line, int Word, int Char)
{

    Uint32 red = SDL_MapRGB(image_surface->format, 255, 0, 0);
    int hh = getHeight(image_surface, w, h);
    int ww = getWidth(image_surface, w, h);
    SDL_Surface *tmp = Cut(image_surface, w, h, ww, hh);
    SDL_Surface *final = getlim(tmp);

    max_h = max(max_h,final->h);
    max_w = max(max_w,final->w);

    printf("h = %d and w = %d\n",max_h,max_w);

    for (int i = h; i < h + hh; i++)
    {
        for (int j = w; j < w + ww; j++)
        {
            put_pixel(image_surface, j, i, red);
        }
    }
    char str[30];
    memset(str, 0, 30);
    sprintf(str, "./Text/Line%d/Word%d/%d.bmp", Line, Word, Char);
    SDL_SaveBMP(final, str);
    SDL_FreeSurface(tmp);
    SDL_FreeSurface(final);
}

int getHeight(SDL_Surface *image_surface, int j, int i)
{
    int height = image_surface->h;
    int h = 0;

    Uint8 r, g, b;
    do
    {
        Uint32 pixel = get_pixel(image_surface, j, i);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        i++;
        h++;
    } while (!(r == 255 && g == 0) && i < height);
    return h - 1;
}

int getWidth(SDL_Surface *image_surface, int j, int i)
{
    int width = image_surface->w;
    int w = 0;

    Uint8 r, g, b;
    do
    {
        Uint32 pixel = get_pixel(image_surface, j, i);
        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
        j++;
        w++;
    } while (!(r == 255 && g == 0) && j < width);
    return w - 1;
}
