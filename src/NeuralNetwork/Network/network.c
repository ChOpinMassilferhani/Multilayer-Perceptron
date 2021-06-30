#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "network.h"

#define max(a, b) (a >= b ? a : b)

struct network *Network_init(
    int input,
    int layers,
    int hidden,
    int output,
    void (*activation)(struct matrice *),
    double (*derivative_function)(double),
    int loaded)
{
    srand(time(NULL));

    struct network *tmp = calloc(1, sizeof(struct network));

    tmp->activation_function = activation;
    tmp->derivative_function = derivative_function;

    tmp->input = matrice_init(input, 1);

    tmp->weight_ih = matrice_init(hidden, input);
    if (loaded != 1)
        randomise_mat(tmp->weight_ih);

    /*    Init input layers    */

    tmp->layers = layers;
    tmp->hiddens = malloc(layers * sizeof(double*));
    tmp->weight_hh = malloc(layers * sizeof(double*));
    tmp->hiddens_biais = malloc(layers * sizeof(double*));
    for (int i = 0; i < layers; i++)
    {
        tmp->hiddens[i] = matrice_init(hidden, 1);
        tmp->hiddens_biais[i] = matrice_init(hidden, 1);
        if (loaded != 1)
            randomise_mat(tmp->hiddens_biais[i]);

        if (i != layers - 1)
        {
            tmp->weight_hh[i] = matrice_init(hidden, hidden);
            if (loaded != 1)
                randomise_mat(tmp->weight_hh[i]);
        }
    }
    /*     Init hidden layers  */

    tmp->weight_ho = matrice_init(output, hidden);
    if (loaded != 1)
        randomise_mat(tmp->weight_ho);

    tmp->output = matrice_init(output, 1);
    tmp->output_biais = matrice_init(output, 1);
    if (loaded != 1)
        randomise_mat(tmp->output_biais);
    /*      Init output layers  */
    return tmp;
}

void sigmoid_activate(struct matrice *cur)
{
    int len = cur->width * cur->height;

    for (int i = 0; i < len; i++)
    {
        cur->mat[i] = sigmoid(cur->mat[i]);
    }
}

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}
double sigmoidprime(double x)
{
    return x * (1 - x);
}

void softmax_activate(struct matrice *cur)
{
    softmax(cur);
}

double softmaxprime(double x)
{
    return x * (1 - x);
}

void softmax(struct matrice *cur)
{
    double maxi = 0;
    int len = cur->height * cur->width;
    for (int i = 0; i < len; i++)
        maxi = max(maxi, cur->mat[i]);

    double sum = 0;
    for (int i = 0; i < len; i++)
    {
        cur->mat[i] = exp(cur->mat[i] - maxi);
        sum += cur->mat[i];
    }

    for (int i = 0; i < len; i++)
        cur->mat[i] = cur->mat[i] / sum;
}

void feedforward(struct network *net)
{
    mult(net->hiddens[0], net->weight_ih, net->input);
    add(net->hiddens[0], net->hiddens[0], net->hiddens_biais[0]);
    sigmoid_activate(net->hiddens[0]);

    for (int i = 1; i < net->layers; i++)
    {
        mult(net->hiddens[i], net->weight_hh[i - 1], net->hiddens[i - 1]);
        add(net->hiddens[i], net->hiddens[i], net->hiddens_biais[i]);
        sigmoid_activate(net->hiddens[i]);
    }

    mult(net->output, net->weight_ho, net->hiddens[net->layers - 1]);
    add(net->output, net->output, net->output_biais);
    net->activation_function(net->output);
}

void backpropagation(struct network *net, int expected, double learning_rate)
{
    double *error_out = calloc(net->output->height, sizeof(double));
    double **error_hidden = calloc(net->layers, sizeof(double *));
    int len_hidden = net->hiddens[0]->height;

    for (int i = 0; i < net->layers; i++)
    {
        error_hidden[i] = calloc(len_hidden, sizeof(double));
    }

    for (int i = 0; i < net->output->height; i++)
    {
        error_out[i] = net->derivative_function(net->output->mat[i]) * (((i == expected) ? 1 : 0) - net->output->mat[i]); //output
    }

    for (int i = 0; i < len_hidden; i++)
    {
        double global = 0.0;
        for (int j = 0; j < net->output->height; j++)
            global += net->weight_ho->mat[i * net->output->height + j] * error_out[j];
        (error_hidden[net->layers - 1])[i] = sigmoidprime(net->hiddens[net->layers - 1]->mat[i]) * global; // last hidden
    }

    for (int layer = net->layers - 2; layer >= 0; layer--)
    {
        for (int i = 0; i < len_hidden; i++)
        {
            double global = 0.0;
            for (int j = 0; j < len_hidden; j++)
            {
                global += net->weight_hh[layer]->mat[i * len_hidden + j] * (error_hidden[layer+1])[j]; // all others hiddens      --problem
            }
            (error_hidden[layer])[i] = sigmoidprime(net->hiddens[layer]->mat[i]) * global;
        }
    }

    for (int i = 0; i < net->output->height; i++)
    {
        for (int j = 0; j < len_hidden; j++)
        {
            net->weight_ho->mat[i * len_hidden + j] += learning_rate * error_out[i] * net->hiddens[net->layers - 1]->mat[j]; // correct weight_ho
        }
        net->output_biais->mat[i] += learning_rate * error_out[i];
    }

    for (int layer = net->layers - 1; layer > 0; layer--)
    {
        for (int i = 0; i < len_hidden; i++)
        {
            for (int j = 0; j < len_hidden; j++)
            {
                net->weight_hh[layer-1]->mat[i * len_hidden + j] += learning_rate * error_hidden[layer][i] * net->hiddens[layer-1]->mat[j];//       --problem
            }
            net->hiddens_biais[layer]->mat[i] += learning_rate * error_hidden[layer][i];
        }
    }

    for (int i = 0; i < len_hidden; i++)
    {
        for (int j = 0; j < net->input->height; j++)
        {
            net->weight_ih->mat[i * net->input->height + j] += learning_rate * error_hidden[0][i] * net->input->mat[j];
        }
        net->hiddens_biais[0]->mat[i] += learning_rate * error_hidden[0][i];
    }

    free(error_out);
    for (int i = 0; i < net->layers; i++)
        free(error_hidden[i]);
    free(error_hidden);    
}

double cost(struct network *net, int index)
{
    double cost = 0;
    int len = net->output->width * net->output->height;
    for (int i = 0; i < len; i++)
    {
        double tmp = net->output->mat[i] - ((i == index) ? 1 : 0);
        cost += pow(tmp, 2);
    }
    return cost / (double)2;
}

int result(struct network *net)
{
    int len = net->output->width * net->output->height;
    int maxi = 0;
    for (int i = 0; i < len; i++)
    {
        maxi = (net->output->mat[maxi] > net->output->mat[i]) ? maxi : i;
    }
    return maxi;
}

void set_input(struct network *net, double *input)
{
    int len = net->input->height;
    for (int i = 0; i < len; i++)
        net->input->mat[i] = input[i];
}
