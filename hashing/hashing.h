#ifndef HASHING_H
#define HASHING_H
#include "../state/state.h"

struct hashtable {
	unsigned int size;
	// should we use a double pointer here?
	void** data;
};
typedef struct hashtable hashtable;

unsigned long hash(unsigned int *values, int n);
unsigned long int hash_state(state s);
hashtable new_hashtable(int size);
void* lookup(hashtable h, int n, void* s);
void* lookup_via(hashtable, void*, unsigned long int (void*));
void insert(hashtable h, int n, void* s);
void insert_via(hashtable, void*, unsigned long int (void*));

#endif
