#ifndef RULE_H
#define RULE_H

// A cellular automata rule, which specifies how new states should evolve from previous ones
struct rule {
	char* name;
};
typedef struct rule rule;

// A data type describing a "neighborhood", a region of influence (usually centered on the target cell) that determines which cells will affect the evolution of the new cell
struct neighborhood {
	int inner_radius;
	int outer_radius;
	unsigned int area;
};
typedef struct neighborhood neighborhood;

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


neighborhood new_neighborhood(int, int);
manifold random_manifold();

#endif
