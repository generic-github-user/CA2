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

#endif
