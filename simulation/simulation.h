#ifndef SIMULATION_H
#define SIMULATION_H
// #include "state.h"
#include "../array/array.h"
#include "../timeinfo.h"

struct state;
typedef struct state state;

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {
	state** states; // List of states contained in this simulation
	int time; // Current timestep of the simulation
	int steps; // Total simulation length
	int compute; // Number of operations used for this simulation
	array ages; // Number of steps for which each living cell has been alive
	int size; // Amount of space dynamically alloated for members of this struct
  timeinfo t;
};
typedef struct simulation simulation;

simulation* new_simulation(state* s, int steps);
void sim_data(simulation s);

void free_sim(simulation*);
void update_sim(simulation*);
void sim_summary(simulation*);
char* sim_info(simulation);
void step(state*, state*, int, int, int*, simulation, int, char);
void simulate(simulation*, int, int, int, int, char, int);

#endif
