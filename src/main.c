#include <stdio.h>
#include "binary.h"
#include "network.h"
#include "save.h"
#include "OCR.h"
#include <err.h>

extern char *values;

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./OCR [new / load]");

    int len_values = strlen(values);
    int input = 40 * 40;
    int layer = 2;
    int hidden = 120;
    int output = len_values;
    struct network *net;

    if (strcmp(argv[1], "new") == 0)
    {
        net = Network_init(input, layer, hidden, output, softmax_activate, softmaxprime, 0);
        learning(net, len_values);
    }
    else
    {
        net = load("Save.txt");
    }
    test_unknow(net);
    save(net);

    free_network(net);
    exit(EXIT_SUCCESS);
}
