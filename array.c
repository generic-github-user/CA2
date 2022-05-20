#include <stdio.h>
#include <stdlib.h>

// #include "vector.h"
#include "array.h"

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
	array a = { rank, shape, size, data, space };
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
