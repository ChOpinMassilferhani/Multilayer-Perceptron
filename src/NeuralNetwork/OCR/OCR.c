#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include "network.h"
#include "matrice.h"
#include "save.h"
#include "image_matrice.h"
#include "pixel_operations.h"
#include "binary.h"
#include "OCR.h"

const char *values = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789,.-!?";

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    if (write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12) == -1)
        errx(EXIT_FAILURE, "Can't clean the Console");
}

void free_infos(struct info **infoarray)
{
    for (int i = 0; i < 67; i++)
    {
        free(infoarray[i]->str);
        free(infoarray[i]);
    }
}

void print_infos(struct info **infoarray)
{
    printf("\n");
    for (int i = 0; i < 26; i++)
    {
        if (infoarray[i]->color == 0)
        {
            printf("\033[1;31m");
            printf("%s", infoarray[i]->str);
            printf("\033[0m");
        }
        else
        {
            printf("\033[0;32m");
            printf("%s", infoarray[i]->str);
            printf("\033[0m");
        }

        printf("         ");

        if (infoarray[26 + i]->color == 0)
        {
            printf("\033[1;31m");
            printf("%s", infoarray[26 + i]->str);
            printf("\033[0m");
        }
        else
        {
            printf("\033[0;32m");
            printf("%s", infoarray[26 + i]->str);
            printf("\033[0m");
        }

        if (i < 15)
        {
            printf("         ");
            if (infoarray[26 * 2 + i]->color == 0)
            {
                printf("\033[1;31m");
                printf("%s", infoarray[26 * 2 + i]->str);
                printf("\033[0m");
            }
            else
            {
                printf("\033[0;32m");
                printf("%s", infoarray[26 * 2 + i]->str);
                printf("\033[0m");
            }
        }
        printf("\n");
    }
}

void learning(struct network *net, int len_values, int iteration)
{
    int k = 0;
    double learning_rate = 0.05;
    char *tlen = malloc(100 * sizeof(char));

    FILE *fp;
    size_t len = 0;

    fp = fopen("../Data/Len.txt", "r");

    if (fp == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    char *tlen2 = NULL;
    if (getline(&tlen2, &len, fp) == -1)
        errx(EXIT_FAILURE, "Can't read");

    int nb_polices = atoi(tlen2);

    free(tlen2);

    struct info **infoarray = calloc(len_values, sizeof(struct info *));
    for (int i = 0; i < len_values; i++)
    {
        infoarray[i] = malloc(sizeof(struct info));
        infoarray[i]->str = calloc(100, sizeof(char));
    }

    double ***learning_data = calloc(nb_polices, sizeof(double *));

    for (int i = 0; i < nb_polices; i++)
    {
        memset(tlen, 0, 100);
        sprintf(tlen, "./Data/Police%d", i);
        learning_data[i] = generate_matrices(tlen);
    }

    free(tlen);

    int cur_police = 0;
    double *input;

    for (int i = 0; i < iteration; i++)
    {

        k = i % len_values;
        if (i == 150000)
            learning_rate = 0.03;
        if (i == 500000)
            learning_rate = 0.01;

        if (k == 0)
        {
            cur_police = (cur_police + 1) % nb_polices;
        }

        input = learning_data[cur_police][k];
        set_input(net, input);

        feedforward(net);

        int out = result(net);
        double cout = cost(net, k);

        memset(infoarray[k]->str, 0, 100);
        sprintf(infoarray[k]->str, "for '%c' we got '%c' |  cost = %lf", values[k], values[out], cout);
        infoarray[k]->color = values[k] == values[out];
        if (k == 0 && i != 0)
        {
            clearScreen();
            print_infos(infoarray);
            printf("\nstep = %d\n", i);
        }

        backpropagation(net, k, learning_rate);
    }

    free_infos(infoarray);
    free(infoarray);
    fclose(fp);

    for (int i = 0; i < nb_polices; i++)
    {
        for (int j = 0; j < 67; j++)
            free(learning_data[i][j]);
        free(learning_data[i]);
    }

    free(learning_data);
}

void theword(struct network *net, int line, int word)
{
    char *tlen = calloc(40, sizeof(char));

    sprintf(tlen, "./Text/Line%d/Word%d/Len.txt", line, word);
    FILE *file;
    file = fopen(tlen, "r");
    size_t len = 0;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    char *tlen2 = NULL;
    if (getline(&tlen2, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");

    int chars = atoi(tlen2);
    free(tlen2);

    for (int cchar = 0; cchar < chars; cchar++)
    {
        sprintf(tlen, "./Text/Line%d/Word%d/%d.bmp", line, word, cchar);
        SDL_Surface *tmp = load_image(tlen);
        double *mat_char = img_to_mat(tmp);
        set_input(net, mat_char);
        feedforward(net);
        free(mat_char);
        SDL_FreeSurface(tmp);
        int output = result(net);
        printf("%c", values[output]);
    }
    fclose(file);
    free(tlen);
}

void theline(struct network *net, int line)
{
    char *tlen = calloc(60, sizeof(char));

    sprintf(tlen, "./Text/Line%d/Len.txt", line);
    FILE *file;
    file = fopen(tlen, "r");
    free(tlen);

    size_t len = 0;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    char *tlen2 = NULL;
    if (getline(&tlen2, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int words = atoi(tlen2);
    free(tlen2);

    for (int word = 0; word < words; word++)
    {
        theword(net, line, word);
        printf(" ");
    }
    fclose(file);
}

void test_unknow(struct network *net)
{
    FILE *file;
    file = fopen("./Text/Len.txt", "r");
    size_t len = 0;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    char *tlen2 = NULL;
    if (getline(&tlen2, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int lines = atoi(tlen2);
    free(tlen2);

    for (int line = 0; line < lines; line++)
    {
        theline(net, line);
        printf("\n");
    }
    fclose(file);
}
