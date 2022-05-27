/* Generated from ./hashing/hashing.c0 at 05/27/2022 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stddef.h>

#include "hashing.h"

// Based on djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned int *values, int n) {
        unsigned long int hash = 5381;

        for (int i=0; i<n; i++) {
            hash = ((hash << 5) + hash) + values[i]; /* hash * 33 + c */
	  }

        return hash;
}

unsigned long int hash_state(state s) {
//	for
//	return hash((unsigned int*) s -> data.data, s -> data.size);
	return hash((unsigned int*) s.data.data, s.data.size);
}

// multidimensional array-based hashtable?
// other open addressing schemes?
// nesting hash tables?

hashtable new_hashtable(int size) {
	hashtable h = {size};
	for (int i=0; i<size; i++) {
		h.data[i] = NULL;
	}
	return h;
}

void* lookup(hashtable h, int n, void* s) {
	int index = hash(s, n) % h.size;
	for (int i=0; i<h.size; i++) {
		void* value = h.data[index + i];
		if (value != NULL) {
			return value;
		}
	}
	return NULL;
}

void* lookup_via(hashtable h, void* v, unsigned long int (*hf)(void*)) {
	int index = hf(v) % h.size;
	for (int i=0; i<h.size; i++) {
		void* value = h.data[index + i];
		if (value != NULL) {
			return value;
		}
	}
	return NULL;
}

void insert(hashtable h, int n, void* s) {
	int index = hash(s, n) % h.size;
	for (int i=0; i<h.size; i++) {
		void* value = h.data[index + i];
		if (value == NULL) {
			h.data[index + 1] = s;
		}
	}
}

void insert_via(hashtable h, void* v, unsigned long int (*hf)(void*)) {
	int index = hf(v) % h.size;
	for (int i=0; i<h.size; i++) {
		void* value = h.data[index + i];
		if (value == NULL) {
			h.data[index + 1] = v;
		}
	}
}

//void remove(hashtable h, state* s) {

//}
