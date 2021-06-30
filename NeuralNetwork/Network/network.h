#ifndef NETWORK_H_
#define NETWORK_H_

# include "../Tools/matrice.h"

struct network
{
    void (*activation_function)(struct matrice*);
    double (*derivative_function)(double);

    struct matrice *input;

    struct matrice *weight_ih;

    int layers;
    struct matrice **hiddens;
    struct matrice **weight_hh;
    struct matrice **hiddens_biais;

    struct matrice *weight_ho;

    struct matrice *output;
    struct matrice *output_biais;
};

void free_network(struct network *net);

struct network *Network_init(int input, int layer, int hidden, int output, void (*activation) (struct matrice*), double (*derivative_function)(double), int loaded);

double sigmoid(double x);

void backpropagation(struct network *net, int expected, double learning_rate);

void feedforward(struct network *net);

double sigmoidprime(double x);

void sigmoid_activate(struct matrice *cur);

void print_matrix(struct matrice *cur);

int result(struct network *net);

void set_input(struct network *net, double *input);

double cost(struct network *net, int index);

void softmax(struct matrice* cur);

double softmaxprime(double x);

void softmax_activate(struct matrice *cur);


#endif
