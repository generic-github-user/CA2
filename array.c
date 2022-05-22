#include <stdio.h>
#include <stdlib.h>

// #include "vector.h"
#include "array.h"

// Create a statically typed function that reduces an array to a single value
#define ARRAY_REDUCE(name,type,op,init) type name(array a) { \
	type output = init;\
	for (int i=0; i<a.size; i++) {\
		output = output op a.data[i];\
	}\
	return output;\
}

#define ARRAY_OP(name,op) array name(array a, array b) {\
	array output = new_array(a.rank, a.shape);\
	for (int i=0; i<a.size; i++) {\
		output.data[i] = a.data[i] op b.data[i];\
	}\
	return output;\
}


// Fill an array with a value
array fill_array(array a, int value) {
	for (int i=0; i<a.size; i++) {
		a.data[i] = value;
	}
	return a;
}

// Initialize an array struct
array new_array(int rank, int* shape) {
	// int* size = malloc(sizeof(int));
	int size = 1;
	for(int i=0; i<rank; i++) {
		size *= shape[i];
	}
	//printf("Initalizing array with size %i \n", size);
	int* data = calloc(size, sizeof(int));
	int space = size * sizeof(int);
	array a = { rank, shape, size, data, space, 0, NULL };
	// a.indices = new_array(
	a.indices = new_list(NULL);
	fill_array(a, 0);
	return a;
};

array vec_to_array(vector v) {
	int s[1] = {3};
	array output = new_array(1, s);
	//output.data = (int*) {v.x, v.y};
	output.data[0] = v.x;
	output.data[1] = v.y;
	output.data[2] = v.z;
	return output;
}

// Convert a series of indices to a corresponding memory address in the internal representation of the array data
int get_coord(array a, vector z) {
	// ?
	// return z.x * a.shape[1] + z.y;

	array w = vec_to_array(z);
	// TODO: make sure this will still work with very large arrays
	int block_size = 1;
	int q = 0;
	for (int i=0; i<3; i++) {
		q += w.data[i] * block_size;
		block_size *= a.shape[i];
		a.compute ++;
	}
	return q;
}

int array_get(array a, vector z) {
	return a.data[get_coord(a, z)];
}

void array_set(array a, vector z, int value) {
	a.data[get_coord(a, z)] = value;
	if (value != 0) {
		vector* pos = malloc(sizeof(vector));
		*pos = vec(z.x, z.y, z.z);
		list_add(a.indices, (void*) pos);
	}
}


// array array_from(int rank, int* shape, void* values) {


//array array_and(array a1, array a2) {

// void map_array(array a, )

void* reduce_array(array a, void* (F)(void*, void*), void* init) {
	void* output = init;
	for (int i=0; i<a.size; i++) {
		output = F(output, &a.data[i]);
	}
	return output;
}

// void* sum(int a, int b) { return (void*) a + b; }
// int array_sum(array a) { return (int) reduce_array(a, sum, 0); }
ARRAY_REDUCE(array_sum, int, +, 0)
double array_mean(array a) {
	return (double) array_sum(a) / (double) a.size;
}

int array_min(array* a) {
	int output = a->data[0];
	for (int i=0; i<a->size; i++) {
		if (a->data[i] < output) {
			output = a->data[i];
		}
		a -> compute ++;
	}
	return output;
}

int array_max(array* a) {
	int output = a->data[0];
	for (int i=0; i<a->size; i++) {
		if (a->data[i] > output) {
			output = a->data[i];
		}
		a -> compute ++;
	}
	return output;
}

ARRAY_OP(array_bsum, +);
ARRAY_OP(array_bdiff, -);
ARRAY_OP(array_bprod, *);
ARRAY_OP(array_bdiv, /);
ARRAY_OP(array_bmod, %);
