#ifndef STATE_H
#define STATE_H

#include <stdio.h>
#include "../simulation/simulation.h"
#include "../array/array.h"

// struct simulation;
// typedef struct simulation simulation;

// A static "frame" of a simulation to which the rules of a cellular automata may be repeatedly applied in a simulation
struct state {
	array data;
	int population;
	double density;
	simulation* sim;
	int* shape;
};
typedef struct state state;

state* new_state(array data, simulation* sim);
void update_state(state* s);
state* random_state(int* shape);
char* state_summary(state s);
int count_neighbors(state source, int x, int y, int* cc);
state* map_neighbors(state s, int* cc);
void print_state(state s, int unicode, char color);
state* clone_state(state s);
void write_state(state s, FILE* fptr);
int states_equal(state a, state b);
void mutate_state(state* s);

state* min_population(state*, int);
state* max_population(state*, int);

#endif