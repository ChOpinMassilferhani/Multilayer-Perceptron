#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "save.h"
#include "../Tools/matrice.h"

void save(struct network *net)
{
    char response[5];
    printf("Save? yes/no\n");
    do
    {
        memset(response, 0, sizeof(response));
        if (fgets(response, sizeof(response), stdin) == NULL)
            errx(EXIT_FAILURE, "Error Buffer");
    } while (strcmp("yes\n", response) != 0 && strcmp("no\n", response) != 0);

    if (strcmp("no\n", response) == 0)
        return;

    FILE *file = fopen("../Save.txt", "w");

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't create the save file");

    int len_input = net->input->height;
    int layer = net->layers;
    int len_hidden = net->hiddens_biais[0]->height;
    int len_output = net->output_biais->height;

    if (net->activation_function == softmax_activate)
        fprintf(file, "SoftMax\n");
    else
        fprintf(file, "Sigmoid\n");

    fprintf(file, "%d\n", len_input);
    fprintf(file, "%d\n", layer);
    fprintf(file, "%d\n", len_hidden);
    fprintf(file, "%d\n", len_output);

    int len_weight_ih = len_input * len_hidden;
    for (int i = 0; i < len_weight_ih; i++)
        fprintf(file, "%lf\n", net->weight_ih->mat[i]);

    int len_weight_hh = len_hidden * len_hidden;
    for (int i = 0; i < layer; i++)
    {
        for (int j = 0; j < len_hidden; j++)
            fprintf(file, "%lf\n", net->hiddens_biais[i]->mat[j]);

        if (i != layer - 1)
            for (int j = 0; j < len_weight_hh; j++)
                fprintf(file, "%lf\n", net->weight_hh[i]->mat[j]);
    }

    int len_weight_ho = len_hidden * len_output;
    for (int i = 0; i < len_weight_ho; i++)
        fprintf(file, "%lf\n", net->weight_ho->mat[i]);

    for (int i = 0; i < len_output; i++)
        fprintf(file, "%lf\n", net->output_biais->mat[i]);

    fclose(file);
}

struct network *load(char *path)
{

    struct network *cur = NULL;
    char *line = NULL;
    char *function = NULL;
    size_t len;

    FILE *file;
    file = fopen(path, "r");

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't open the save file");

    if (getline(&function, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");

    if (getline(&line, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int len_input = atoi(line);

    if (getline(&line, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int layer = atoi(line);

    if (getline(&line, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int len_hidden = atoi(line);

    if (getline(&line, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int len_output = atoi(line);

    if (strcmp("SoftMax\n", function) == 0)
        cur = Network_init(len_input, layer, len_hidden, len_output, softmax_activate, softmaxprime, 1);
    else
        cur = Network_init(len_input, layer, len_hidden, len_output, sigmoid_activate, sigmoidprime, 1);

    for (int i = 0; i < len_hidden; i++)
    {
        for (int j = 0; j < len_input; j++)
        {
            if (getline(&line, &len, file) == -1)
                errx(EXIT_FAILURE, "Can't read");
            cur->weight_ih->mat[i * len_input + j] = atof(line);
        }
    }

    int len_weight_hh = len_hidden * len_hidden;
    for (int i = 0; i < layer; i++)
    {
        for (int j = 0; j < len_hidden; j++)
        {
            if (getline(&line, &len, file) == -1)
                errx(EXIT_FAILURE, "Can't read");
            cur->hiddens_biais[i]->mat[j] = atof(line);
        }
        if (i != layer - 1)
            for (int j = 0; j < len_weight_hh; j++)
            {
                if (getline(&line, &len, file) == -1)
                    errx(EXIT_FAILURE, "Can't read");
                cur->weight_hh[i]->mat[j] = atof(line);
            }
    }

    for (int i = 0; i < len_output; i++)
    {
        for (int j = 0; j < len_hidden; j++)
        {
            if (getline(&line, &len, file) == -1)
                errx(EXIT_FAILURE, "Can't read");
            cur->weight_ho->mat[i * len_hidden + j] = atof(line);
        }
    }

    for (int i = 0; i < len_output; i++)
    {
        if (getline(&line, &len, file) == -1)
            errx(EXIT_FAILURE, "Can't read");
        cur->output_biais->mat[i] = atof(line);
    }

    free(function);
    free(line);
    if(fclose(file) != 0)
        errx(EXIT_FAILURE,"Fail while closing the file");

    return cur;
}
