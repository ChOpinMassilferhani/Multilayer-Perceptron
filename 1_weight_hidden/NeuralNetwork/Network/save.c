# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <string.h>
# include "save.h"
# include "../Tools/matrice.h"

void save(struct network *net)
{
    char response[5];
    printf("Save? yes/no\n");
    do
    {
        memset(response,0,sizeof(response));
        if(fgets(response,sizeof(response),stdin) == NULL)
		errx(EXIT_FAILURE,"Error Buffer");
    } while (strcmp("yes\n",response) != 0 && strcmp("no\n",response) != 0);
    
    FILE *file = fopen("Save.txt", "w");

    if (file == NULL)
        errx(EXIT_FAILURE, "Can't create the save file");

    int len_input = net->input->height;
    int len_hidden = net->hidden_biais->height;
    int len_output = net->output_biais->height;

    if (net->activation_function == softmax_activate)
        fprintf(file, "SoftMax\n");
    else
        fprintf(file, "Sigmoid\n");

    fprintf(file, "%d\n", len_input);
    fprintf(file, "%d\n", len_hidden);
    fprintf(file, "%d\n", len_output);

    for (int i = 0; i < len_hidden; i++)
        fprintf(file, "%lf\n", net->hidden_biais->mat[i]);

    int len_weight_ih = len_input * len_hidden;
    for (int i = 0; i < len_weight_ih; i++)
        fprintf(file, "%lf\n", net->weight_ih->mat[i]);

    for (int i = 0; i < len_output; i++)
        fprintf(file, "%lf\n", net->output_biais->mat[i]);

    int len_weight_ho = len_hidden * len_output;
    for (int i = 0; i < len_weight_ho; i++)
        fprintf(file, "%lf\n", net->weight_ho->mat[i]);

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
    int len_hidden = atoi(line);

    if (getline(&line, &len, file) == -1)
        errx(EXIT_FAILURE, "Can't read");
    int len_output = atoi(line);

    if (strcmp("SoftMax\n", function) == 0)
        cur = Network_init(len_input, len_hidden, len_output, softmax_activate, softmaxprime, 1);
    else
        cur = Network_init(len_input, len_hidden, len_output, sigmoid_activate, sigmoidprime, 1);

    for (int i = 0; i < len_hidden; i++)
    {
        if (getline(&line, &len, file) == -1)
            errx(EXIT_FAILURE, "Can't read");
        cur->hidden_biais->mat[i] = atof(line);
    }

    for (int i = 0; i < len_hidden; i++)
    {

        for (int j = 0; j < len_input; j++)
        {
            if (getline(&line, &len, file) == -1)
                errx(EXIT_FAILURE, "Can't read");
            cur->weight_ih->mat[i * len_input + j] = atof(line);
        }
    }

    for (int i = 0; i < len_output; i++)
    {
        if (getline(&line, &len, file) == -1)
            errx(EXIT_FAILURE, "Can't read");
        cur->output_biais->mat[i] = atof(line);
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

    return cur;
}
