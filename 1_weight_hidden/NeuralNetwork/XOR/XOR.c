# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <err.h>
# include "../Network/network.h"
# include "../Tools/matrice.h"
# include "../Network/save.h"

int main(int argc, char **argv)
{
    if(argc != 2 && argc != 3)
        errx(EXIT_FAILURE,"Usage : ./XOR [load] [don't load]");
    struct network *net = NULL;
    
    if(strcmp("load",argv[1]) == 0)
        net = load("./Save.txt");
    else
    {
        if(strcmp("s",argv[2]) == 0)
            net = Network_init(2, 3, 2,softmax_activate,softmaxprime,0);
        else
            net = Network_init(2, 3, 2,sigmoid_activate,sigmoidprime,0);
        
        
    }
    
    double learning_rate = 0.01;
    int expected;
    double yes[] = {1,0};
    double yes2[] = {0,1};
    double no[] = {0,0};
    double no2[] = {1,1};

    for (int i = 0; i < 10000000; i++)
    {

        if (i % 4 == 0)
        {
            set_input(net,yes);
            expected = 1;
        }
        else
        {
            if (i % 4 == 1)
            {
                set_input(net,yes2);
                expected = 1;
            }
            else
            {
                if (i % 4 == 2)
                {
                    set_input(net,no);
                    expected = 0;
                }
                else
                {
                    set_input(net,no2);
                    expected = 0;
                }
            }
        }

        feedforward(net);

        int out = result(net);
        double cout = cost(net, expected);
        printf("for %f %f | We get = %d | cost = %f | step = %d\n", net->input->mat[0], net->input->mat[1], out, cout, i);

        backpropagation(net, expected,learning_rate);
    }
    save(net);

    return EXIT_SUCCESS;
}
