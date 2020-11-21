#ifndef SAVE_H_
#define SAVE_H_

# include "network.h"

void save(struct network *net);

struct network *load(char *path);

#endif