/* Generated from ./simulation/simulation.c0 at 05/25/2022, 03:26:15 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../state/state.h"
#include "../helpers/helpers.h"
#include "../mainheaders.h"

// should s be a state pointer?
simulation* new_simulation(state s, int steps) {
	simulation* sim = malloc(sizeof(simulation));
	s.sim = sim;
	// TODO: check that we aren't calling sizeof on pointers
	sim->states = (state*) calloc(steps, sizeof(state));
	sim->size = steps * sizeof(state);
	// TODO: sum memory allocated by each state instance
	sim->states[0] = s;

	sim->time = 1;
	sim->steps = steps;
	sim->compute = 0;

	sim->ages = new_array(2, s.data.shape);
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

void sim_summary(simulation* s) {
	fprintf(stdout, "Simulation (%i steps) \n\n", s -> steps);
	fprintf(stdout, "Population -- %s\n", microplot(*s));
	for (int i=0; i<s -> steps; i++) {
		fprintf(stdout, "  Step %i: %i \n", i, (s -> states)[i].population);
	}
}

char* sim_info(simulation s) {
	char* result = calloc(100, sizeof(char));
//	strcat(result, "Simulation { Length");
//	strcat(result, itoa(s.steps));
	snprintf(result, 100, CYAN "Simulation { length: %i, compute: %i, size: %i }" RESET, s.steps, s.compute, s.size);
	return result;
}

//delta

// Simulate a single step of a cellular automata rule, given the previous and current states (the new state is modified in place)
void step(state* s, state* p, int i, int show, int* cc, simulation sim, int unicode, char color) {
	if (show) {
		printf("Simulating frame %i \n", i+1);
	}
	int neighbors = 0;
	for (int x=0; x<s->shape[0]; x++) {
		for (int y=0; y<s->shape[1]; y++) {
			array_set(s -> data, vec(x, y, 0), array_get(p -> data, vec(x, y, 0)));
			sim.ages.data[get_coord(sim.ages, vec(x, y, 0))] ++;
//			compute ++;
			(*cc) ++;
		}
	}

	for (int x=0; x<s->shape[0]; x++) {
		for (int y=0; y<s->shape[1]; y++) {
			neighbors = count_neighbors(*p, x, y, cc);
			(*cc) ++;
			if (neighbors == 3) {
				array_set(s -> data, vec(x, y, 0), 1);
			}
			if (neighbors < 2 || neighbors > 3) {
				array_set(s -> data, vec(x, y, 0), 0);
				array_set(sim.ages, vec(x, y, 0), 0);
			}
		}
	}
	update_state(s);

	if (show) {
		print_state(*s, unicode, color);
		printf("\n");
	}
	fflush(stdout);
	if (show) {
		usleep(200000);
	}
}



// note: don't pass by value?!?!
// Simulate n steps of a cellular automaton (last four arguments are print settings)
void simulate(simulation* sim, int n, int show, int level, int unicode, char color, int progress) {
	int prog = 0;
	printx(level+1, "");
	printf("Simulating %i iterations\n", n-1);
	printx(level+1, "");
	if (progress) { printf("["); }
	for (int i=0; i<n-1; i++) {
		//state* p = &(sim->states)[(sim->time)-1];
		state* p = &(sim->states)[(sim->time)-1];
		(sim -> states)[sim -> time] = *new_state(new_array(2, p -> data.shape), sim);
		step(&(sim->states)[sim->time], p, i, show, &(sim -> compute), *sim, unicode, color);

		(sim -> time) ++;
		if (progress) {
			int q = 40 * ((double) i / (double) n);
			if (q > prog) {
				printf("#");
				prog = q;
			}
		}
		(sim -> compute) ++;
	}
	if (progress) { printf("]\n"); }

	char temp[50];
	sprintf(temp, "Simulation complete; compute usage was %i \n", sim -> compute);
	printx(level+1, temp);
//	free(temp);
}
