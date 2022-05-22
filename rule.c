#include <stdlib.h>
#include "rule.h"

neighborhood new_neighborhood(int ir, int or) {
	neighborhood N = {ir, or};
	int id = ir*2-1;
	int od = or*2-1;
	N.area = (od*od) - (id*id);
	return N;
}

// A type of cellular automata rule that uses only total counts of "live" cells in a neighborhood to generate the next state
struct totalistic {
	neighborhood* N;
	int** values;
};
typedef struct totalistic totalistic;

// struct name_group
// struct lattice

// TODO: add support for more exotic surfaces (like spheres)
struct manifold {
	char* lattice;
	int dimensions;
	int* shape;

	// Edge behaviors are described as sequential pairs of characters indicating the two directions of motion along each axis, with the following representation:
	// e: expand boundaries to fit the automaton's activity
	// w: wrap from one edge to the opposite (e.g., global wrapping on a 2D plane generates a torus)
	// i: wrap edges, inverted (as with a Mobius strip)
	//
	char edges;
};
typedef struct manifold manifold;

// TODO: type description classes
manifold random_manifold() {
	char edge_types[3] = "ewi";
	int dim = rand() % 4;
//	int* shape = random_array
	int* shape = calloc(dim, sizeof(int));
	for (int i=0; i<dim; i++) {
		shape[i] = (rand() % 95) + 5;
	}
	manifold m = {"grid", dim, shape, edge_types[rand() % 3]};
	return m;
}

// manifold mutate_manifold(manifold m) {

