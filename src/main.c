#include <stdio.h>
#include "network.h"
#include "save.h"
#include "OCR.h"
#include "segmentation.h"
#include <err.h>

extern char *values;

int main(int argc, char **argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Usage: ./OCR [new|load] <image path>");

    SDL_Surface *image_surface = load_image(argv[2]);
    segmentation(image_surface);
    SDL_FreeSurface(image_surface);

    printf("segmentation ok !\n");


    int len_values = strlen(values);
    int input = 40 * 40;
    int layer = 4;
    int hidden = 128;
    int output = len_values;
    int iteration = 500000;
    struct network *net;

    if (strcmp(argv[1], "new") == 0)
    {
        net = Network_init(input, layer, hidden, output, softmax_activate, softmaxprime, 0);
        learning(net, len_values, iteration);
    }
    else
    {
        net = load("../Save.txt");
    }
    test_unknow(net);
    save(net);

    free_network(net);
    exit(EXIT_SUCCESS);
}
