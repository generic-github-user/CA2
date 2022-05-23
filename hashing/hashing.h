#ifndef HASHING_H
#define HASHING_H
#include "state.h"

struct hashtable {
	unsigned int size;
	// should we use a double pointer here?
	state** data;
};
typedef struct hashtable hashtable;

unsigned long hash(unsigned int *values, int n);
unsigned long int hash_state(state s);
hashtable new_hashtable(int size);
state* lookup(hashtable h, state s);
void insert(hashtable h, state* s);

#endif
