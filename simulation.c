#include <stdlib.h>
#include <stdio.h>
#include "state.h"

// should s be a state pointer?
simulation new_simulation(state s, int steps) {
	simulation sim;
	sim.states = (state*) calloc(steps, sizeof(state));
	sim.size = steps * sizeof(state);
	// TODO: sum memory allocated by each state instance
	sim.states[0] = s;

	sim.time = 1;
	sim.steps = steps;
	sim.compute = 0;

	sim.ages = new_array(2, s.data.shape);

//	printf("Created new simulation
	return sim;
}
