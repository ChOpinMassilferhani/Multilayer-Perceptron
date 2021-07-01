#ifndef __OCR_H_
#define __OCR_H_

struct info
{
    char *str;
    int color;
};


char getChar(int response);

void clearScreen();

void print_infos(struct info **infoarray);

void learning(struct network *net, int len_values);

void theword(struct network *net, int lines, int words);

void theline(struct network *net, int lines);

void test_unknow(struct network *net);



#endif
