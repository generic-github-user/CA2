#ifndef SIMULATION_H
#define SIMULATION_H
// #include "state.h"
#include "../array/array.h"

struct state;
typedef struct state state;

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {
	state** states;
	int time, steps, compute;
	array ages;
	// Amount of space dynamically alloated for members of this struct
	int size;
};
typedef struct simulation simulation;

simulation* new_simulation(state* s, int steps);
void sim_data(simulation s);

void free_sim(simulation*);

void sim_summary(simulation*);
char* sim_info(simulation);
void step(state*, state*, int, int, int*, simulation, int, char);
void simulate(simulation*, int, int, int, int, char, int);

#endif
