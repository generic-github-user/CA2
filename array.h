#ifndef ARRAY_H
#define ARRAY_H

#include "vector.h"

// A general array struct for multidimensional arrays
struct array {
	int rank;
	int* shape;
	int size;
	int* data;
	int space;
	int compute;
};
typedef struct array array;

array fill_array(array a, int value);
array new_array(int rank, int* shape);
array vec_to_array(vector v);
int get_coord(array a, vector z);
int array_get(array a, vector z);
void array_set(array a, vector z, int value);
void* reduce_array(array a, void* (F)(void*, void*), void* init);

#endif
