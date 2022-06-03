#ifndef HELPERS_H
#define HELPERS_H

#include "../simulation/simulation.h"

int inrange(int x, int n, int m);
void print_timestamp();
void printx(int, const char*, ...);
char* microplot(simulation);
int streq(char*, char*);
//void assert(int, char*);
void assert(int);

#endif
