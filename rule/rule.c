/* Generated from rule/rule.c0 at 06/05/2022 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdlib.h>
#include "rule.h"

neighborhood new_neighborhood(int ir, int or) {
	neighborhood N = {ir, or};
	int id = ir*2-1;
	int od = or*2-1;
	N.area = (od*od) - (id*id);
	return N;
}

// Deallocate a manifold struct and its members
void free_manifold(manifold* m) {
	free(m -> shape);
	free(m);
}

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

