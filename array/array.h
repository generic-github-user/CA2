#ifndef ARRAY_H
#define ARRAY_H
#include <stdio.h>

#include "../vector/vector.h"
#include "../graph/graph.h"

// A general array struct for multidimensional arrays
struct array {
	int rank;
	int* shape;
	int size;
	int* data;
	int space;
	int compute;
	node* indices;
};
typedef struct array array;

array fill_array(array a, int value);
array new_array(int rank, int* shape);
array vec_to_array(vector v);
int get_coord(array a, vector z);
int array_get(array a, vector z);
void array_set(array a, vector z, int value);
void* reduce_array(array a, void* (F)(void*, void*), void* init);

int array_sum(array a);
double array_mean(array a);
int array_min(array* a);
int array_max(array* a);

array array_bsum(array a, array b);
array array_bdiff(array a, array b);
array array_bprod(array a, array b);
array array_bdiv(array a, array b);
array array_bmod(array a, array b);

array copy_array(array);
vector array_to_vec(array);
array array_slice(array, array, array, int);

void fill_slice(array*, vector, vector, int);
void write_array(array, FILE*);

#endif
