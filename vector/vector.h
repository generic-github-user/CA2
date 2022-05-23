#ifndef VECTOR_H
#define VECTOR_H

// A vector or coordinate
struct vector {
	int x, y, z;
};
typedef struct vector vector;

vector vec(int x, int y, int z);

#endif
