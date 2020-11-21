# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <time.h>
# include <stdlib.h>
# include "network.h"

#define max(a, b) (a >= b ? a : b)

struct network *Network_init(
    int input,
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

    tmp->hidden = matrice_init(hidden, 1);
    tmp->hidden_biais = matrice_init(hidden, 1);
    if (loaded != 1)
        randomise_mat(tmp->hidden_biais);

    tmp->weight_ho = matrice_init(output, hidden);
    if (loaded != 1)
        randomise_mat(tmp->weight_ho);

    tmp->output = matrice_init(output, 1);
    tmp->output_biais = matrice_init(output, 1);
    if (loaded != 1)
        randomise_mat(tmp->output_biais);

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
    mult(net->hidden, net->weight_ih, net->input);
    add(net->hidden, net->hidden, net->hidden_biais);
    sigmoid_activate(net->hidden);

    mult(net->output, net->weight_ho, net->hidden);
    add(net->output, net->output, net->output_biais);
    net->activation_function(net->output);
}

void backpropagation(struct network *net, int expected, double learning_rate)
{
    double *error_out = calloc(net->output->height, sizeof(double));
    double *error_hidden = calloc(net->hidden->height, sizeof(double));

    for (int i = 0; i < net->output->height; i++)
    {
        error_out[i] = sigmoidprime(net->output->mat[i]) * (((i == expected) ? 1 : 0) - net->output->mat[i]);
    }

    for (int i = 0; i < net->hidden->height; i++)
    {
        double global = 0.0;
        for (int j = 0; j < net->output->height; j++)
            global += net->weight_ho->mat[i * net->output->height + j] * error_out[j];
        error_hidden[i] = net->derivative_function(net->hidden->mat[i]) * global; // to go back to the sigmoid just change softmax to sigmoid and softmaxderiv to sigmoidderiv
    }

    for (int i = 0; i < net->output->height; i++)
    {
        for (int j = 0; j < net->hidden->height; j++)
        {
            net->weight_ho->mat[i * net->hidden->height + j] += learning_rate * error_out[i] * net->hidden->mat[j];
        }
        net->output_biais->mat[i] += learning_rate * error_out[i];
    }

    for (int i = 0; i < net->hidden->height; i++)
    {
        for (int j = 0; j < net->input->height; j++)
        {
            net->weight_ih->mat[i * net->input->height + j] += learning_rate * error_hidden[i] * net->input->mat[j];
        }
        net->hidden_biais->mat[i] += learning_rate * error_hidden[i];
    }
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
