#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "helpers.h"
#include "array.h"

#define PTR_REDUCE(name,property,op) state name(state* states, int n) {\
	state output = states[0];\
	for (int i=0; i<n; i++) {\
		/* if ((states[i] -> property) op (output -> property)) {*/\
		if (states[i].property op output.property) {\
			output = states[i];\
		}\
	}\
	return output;\
}

extern char* COLOR_ORDER[6];
// char* RESET;
#define RESET   "\x1b[0m"

state new_state(array data, simulation* sim) {
	state s = {data, 0, 0, sim};
	s.shape = s.data.shape;
	return s;
}

void update_state(state* s) {
	(s -> population) = array_sum(s -> data);
	// should s -> data be a pointer?
	(s -> density) = (double) array_sum(s -> data) / (double) (s -> data).size;
}

// Generate a random state
state random_state(int* shape) {
	state result = {new_array(2, shape), 0, 0, NULL};
	for (int x=0; x<shape[0]; x++) {
		for (int y=0; y<shape[1]; y++) {
			array_set(result.data, vec(x, y, 0), rand() % 2);
			// compute ++;
		}
	}
	result.shape = shape;
	update_state(&result);
	return result;
}

// Render information about a state to a string
char* state_summary(state s) {
	// temporary
	char* output = calloc(s.data.size+s.data.shape[0]+1, sizeof(char));
	int i = 0;
	for (int x=0; x<s.data.shape[0]; x++) {
		for (int y=0; y<s.data.shape[1]; y++) {
			char c = array_get(s.data, vec(x, y, 0)) ? '#' : ' ';
			output[i++] = c;
		}
		output[i++] = '\n';
	}
	output[i++] = '\0';
	return output;
}

// Count neighbor cells given a state and coordinate
int count_neighbors(state source, int x, int y, int* cc) {
	int neighbors = 0;
	int c, d;
	for (int a=-1; a<=1; a++) {
		for (int b=-1; b<=1; b++) {
			// Exclude target cell
			if (a!=0 || b!=0) {
				c = x+a;
				d = y+b;
				if (inrange(c, 0, source.shape[0]-1) && inrange(d, 0, source.shape[1]-1)) {
					neighbors += array_get(source.data, vec(c, d, 0));
				}
			}
			// compute ++;
			(*cc) ++;
		}
	}
	return neighbors;
}

state map_neighbors(state s, int* cc) {
	int neighbors;
	state counts = {new_array(2, s.data.shape)};
	for (int x=0; x<s.shape[0]; x++) {
		for (int y=0; y<s.shape[1]; y++) {
			neighbors = count_neighbors(s, x, y, cc);
			array_set(counts.data, vec(x, y, 0), neighbors);
			//(*cc) ++;
		}
	}
	return counts;
}

void print_state(state s, int unicode, char color) {
	// printf("Total compute: %i \n", compute);
	printf("Population: %i \n", array_sum(s.data));
	printf("\n");
	int c = 0;
	//int min = array_min(
	for (int x=0; x<s.shape[0]; x++) {
		for (int y=0; y<s.shape[1]; y++) {
			c = 0;
			if (color == 'a' && s.sim != NULL) {
				int age = array_get(s.sim -> ages, vec(x, y, 0));
				if (age > 1) {
					if (age > 7) { age = 7; }
					c = 1;
					printf("%s", COLOR_ORDER[age-2]);
				}
			}
			printf(
				array_get(s.data, vec(x, y, 0)) ?
				(unicode ? "██" : "*") :
				(unicode ? "  " : " "));
			if (c) { printf("%s", RESET); }
		}
		printf("\n");
	}
}

state* clone_state(state s) {
	//state clone = (state) {new_array(2, s.data.shape)};
	state* clone = malloc(sizeof(state));
	*clone = new_state(new_array(2, s.data.shape), s.sim);
	for (int x=0; x<s.shape[0]; x++) {
		for (int y=0; y<s.shape[1]; y++) {
			array_set(clone -> data, vec(x, y, 0), array_get(s.data, vec(x, y, 0)));
			// compute ++;
		}
	}
	return clone;
}

void write_state(state s, FILE* fptr) {
	fprintf(fptr, "Population: %i \n", array_sum(s.data));
	fprintf(fptr, "Density: %f \n", (double) array_sum(s.data) / (double) s.data.size);
	int cc = 0;
	fprintf(fptr, "Avg. neighbors: %f \n", array_mean(map_neighbors(s, &cc).data));

	char* summary = state_summary(s);
	fprintf(fptr, "%s", summary);
	free(summary);
}

int states_equal(state a, state b) {
	// TODO: check that states are of the same size
	for (int x=0; x<a.shape[0]; x++) {
		for (int y=0; y<a.shape[1]; y++) {
			if (array_get(a.data, vec(x, y, 0)) != array_get(b.data, vec(x, y, 0))) {
				return 0;
			}
		}
	}
	return 1;
}

void mutate_state(state* s) {
	array_set(s -> data, vec(rand() % 30, rand() % 30, 0), rand() % 2);
}


PTR_REDUCE(max_population, population, >);
PTR_REDUCE(min_population, population, <);
