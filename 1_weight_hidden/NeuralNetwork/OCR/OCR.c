#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include "../Network/network.h"
#include "../Tools/matrice.h"
#include "../Network/save.h"
#include "../Tools/image_matrice.h"
#include "../../ImageProcessing/binary.h"

const char *values = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789,.-!?";

struct info
{
    char *str;
    int color;
};

void clearScreen()
{
    const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    if (write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12) == -1)
        errx(EXIT_FAILURE, "Can't clean the Console");
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

void learning(struct network *net, int len_values)
{
    int k;
    double learning_rate = 0.01;
    char *path = "./Data";

    double **learning_data = generate_matrices(path);

    struct info **infoarray = calloc(len_values, sizeof(struct info *));

    for (int i = 0; i < 500000; i++)
    {
        if (i == 150000)
            learning_rate = 0.03;
        else if (i == 500000)
            learning_rate = 0.01;
        else
            learning_rate = 0.05;

        k = i % len_values;
        double *input = learning_data[k];
        set_input(net, input);

        feedforward(net);

        int out = result(net);
        double cout = cost(net, k);

        char *tmp_string = malloc(100 * sizeof(char));

        sprintf(tmp_string, "for '%c' we got '%c' |  cost = %lf", values[k], values[out], cout);
        struct info *cur = malloc(sizeof(struct info));
        cur->str = tmp_string;
        cur->color = values[k] == values[out];
        infoarray[k] = cur;
        if (k == 0 && i != 0)
        {
            clearScreen();
            print_infos(infoarray);
            printf("\nstep = %d\n", i);
        }

        backpropagation(net, k, learning_rate);
    }
}

void theword(struct network *net, int line, int word)
{
    char *tlen = calloc(60, sizeof(char));

    sprintf(tlen, "./Text/Line%d/Word%d/Len.txt", line, word);
    FILE *file;
    file = fopen(tlen, "r");
    size_t len;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    if (getline(&tlen, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");

    int chars = atoi(tlen);

    for (int cchar = 0; cchar < chars; cchar++)
    {
        sprintf(tlen, "./Text/Line%d/Word%d/%d.bmp", line, word, cchar);
        SDL_Surface *tmp = load_image(tlen);
        double *mat_char = img_to_mat(tmp);
        set_input(net, mat_char);
        feedforward(net);
        int output = result(net);
        printf("%c", values[output]);
    }
    free(tlen);
}

void theline(struct network *net, int line)
{
    char *tlen = calloc(60, sizeof(char));

    sprintf(tlen, "./Text/Line%d/Len.txt", line);
    FILE *file;
    file = fopen(tlen, "r");
    size_t len;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    if (getline(&tlen, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");

    int words = atoi(tlen);

    for (int word = 0; word < words; word++)
    {
        sprintf(tlen, "./Text/Line%d/Word%d", line, word);
        theword(net, line, word);
        printf(" ");
    }
    free(tlen);
}

void test_unknow(struct network *net)
{

    char *tlen = calloc(60, sizeof(char));

    sprintf(tlen, "./Text/Len.txt");
    FILE *file;
    file = fopen(tlen, "r");
    size_t len;

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't get the Data lenght file errno [ %s ]= %d", strerror(errno), errno);

    if (getline(&tlen, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");

    int lines = atoi(tlen);

    for (int line = 0; line < lines; line++)
    {
        sprintf(tlen, "./Text/Line%d", line);
        theline(net, line);
        printf("\n");
    }
    free(tlen);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./OCR [new / load]");

    int len_values = strlen(values);
    int input = 40 * 40;
    int hidden = 120;
    int output = len_values;

    if (strcmp(argv[1], "new") == 0)
    {
        struct network *net = Network_init(input, hidden, output, softmax_activate, softmaxprime, 0);
        learning(net, len_values);
        save(net);
    }
    else
    {
        struct network *net = load("Save.txt");
        test_unknow(net);
    }

    exit(EXIT_SUCCESS);
}
