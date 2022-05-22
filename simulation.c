#include <stdlib.h>
#include <stdio.h>
#include "state.h"

// should s be a state pointer?
simulation new_simulation(state s, int steps) {
	simulation sim;
	// TODO: check that we aren't calling sizeof on pointers
	sim.states = (state*) calloc(steps, sizeof(state));
	sim.size = steps * sizeof(state);
	// TODO: sum memory allocated by each state instance
	sim.states[0] = s;

	sim.time = 1;
	sim.steps = steps;
	sim.compute = 0;

	sim.ages = new_array(2, s.data.shape);
	// TODO: check this
	//s.sim = &sim;

//	printf("Created new simulation
	return sim;
}

void sim_data(simulation s) {
	char* headers[] = {"Step", "Population"};
	printf("%15s\t%15s\n\n", headers[0], headers[1]);
	for (int i=0; i<s.steps; i++) {
		state st = s.states[i];
		printf("%15d\t%15d\n", i+1, st.population);
	}
}
