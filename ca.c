#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Create a statically typed function that reduces an array to a single value
#define ARRAY_REDUCE(name,type,op,init) type name(struct array a) { \
	type output = init;\
	for (int i=0; i<a.size; i++) {\
		output = output op a.data[i];\
	}\
	return output;\
}

// Example commands

// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort population desc > write test.txt, save test.ca2
// abbr DEFAULT "limit compute 10e9 time 60"
// abbr activity "accumulate difference time"
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort activity desc > write activity.txt, save activity.ca2
// randomstate -wh 50 -t torus, randomrule -n 100 -c totalistic -r 1 > simulate $ -i 100 > write sample.txt
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 500 > sort area desc > write area.txt, save area.ca2
// snowflake > trace cgol > select -n 20 > write backtracking.txt

int compute;

// Clip an integer to a range (modify in-place)
int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

// A vector or coordinate
struct vector {
	int x, y;
};

struct vector vec(int x, int y) {
	return (struct vector) { x, y };
}

struct manifold {

};

// A general array struct for multidimensional arrays
struct array {
	int rank;
	int* shape;
	int size;
	int* data;
};

// Fill an array with a value
struct array fill_array(struct array a, int value) {
	for (int i=0; i<a.size; i++) {
		a.data[i] = value;
		compute ++;
	}
	return a;
}

// Initialize an array struct
struct array new_array(int rank, int* shape) {
	// int* size = malloc(sizeof(int));
	int size = 1;
	for(int i=0; i<rank; i++) {
		size *= shape[i];
		compute ++;
	}
	//printf("Initalizing array with size %i \n", size);
	//int data[size] = {0};
	//int data[size];
	int* data = calloc(size, sizeof(int));
	struct array a = { rank, shape, size, data };
	fill_array(a, 0);
	return a;
};

// Convert a series of indices to a corresponding memory address in the internal representation of the array data
int get_coord(struct array a, struct vector z) {
	// ?
	return z.x * a.shape[0] + z.y;
}

int array_get(struct array a, struct vector z) {
	return a.data[get_coord(a, z)];
}

void array_set(struct array a, struct vector z, int value) {
	a.data[get_coord(a, z)] = value;
}

// struct array array_from(int rank, int* shape, void* values) {


//struct array array_and(struct array a1, struct array a2) {

// void map_array(struct array a, )

void* reduce_array(struct array a, void* (F)(void*, void*), void* init) {
	void* output = init;
	for (int i=0; i<a.size; i++) {
		output = F(output, &a.data[i]);
	}
	return output;
}

// void* sum(int a, int b) { return (void*) a + b; }
// int array_sum(struct array a) { return (int) reduce_array(a, sum, 0); }
ARRAY_REDUCE(array_sum, int, +, 0)
double array_mean(struct array a) {
	return array_sum(a) / a.size;
}

// A static "frame" of a simulation to which the rules of a cellular automata may be repeatedly applied in a simulation
struct state {
	struct array data;
};

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {
	struct state* states;
	int time, steps;
};

// should s be a state pointer?
struct simulation new_simulation(struct state s, int steps) {
	struct simulation sim;
	sim.states = (struct state*) calloc(steps, sizeof(struct state));
	sim.states[0] = s;
	sim.time = 1;
	sim.steps = steps;
//	printf("Created new simulation
	return sim;
}

// Generate a random state
struct state random_state(int* shape) {
	struct state result = {new_array(2, shape)};
	for (int x=0; x<shape[0]; x++) {
		for (int y=0; y<shape[1]; y++) {
			array_set(result.data, vec(x, y), rand() % 2);
			compute ++;
		}
	}
	return result;
}

// Render information about a state to a string
char* state_summary(struct state s) {
	// temporary
	char* output = calloc(s.data.size+s.data.shape[0]+1, sizeof(char));
	int i = 0;
	for (int x=0; x<s.data.shape[0]; x++) {
		for (int y=0; y<s.data.shape[1]; y++) {
			char c = array_get(s.data, vec(x, y)) ? '#' : ' ';
			output[i++] = c;
		}
		output[i++] = '\n';
	}
	output[i++] = '\0';
	return output;
}

int inrange(int x, int n, int m) {
	return x >= n && x <= m;
}

// Count neighbor cells given a state and coordinate
int count_neighbors(struct state source, int x, int y) {
	int neighbors = 0;
	int c, d;
	for (int a=-1; a<=1; a++) {
		for (int b=-1; b<=1; b++) {
			// Exclude target cell
			if (a!=0 || b!=0) {
				c = x+a;
				d = y+b;
				if (inrange(c, 0, 29) && inrange(d, 0, 29)) {
					neighbors += array_get(source.data, vec(c, d));
				}
			}
			compute ++;
		}
	}
	return neighbors;
}

void print_state(struct state s) {
	printf("Total compute: %i \n", compute);
	printf("Population: %i \n", array_sum(s.data));
	printf("\n");
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			printf(array_get(s.data, vec(x, y)) ? "*" : " ");
		}
		printf("\n");
	}
}

void step(struct state s, struct state p, int i, int show) {
	if (show) {
		printf("Simulating frame %i \n", i+1);
	}
	int neighbors = 0;
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array_set(s.data, vec(x, y), array_get(p.data, vec(x, y)));
			compute ++;
		}
	}

	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			neighbors = count_neighbors(p, x, y);
			if (neighbors == 3) { array_set(s.data, vec(x, y), 1); }
			if (neighbors < 2 || neighbors > 3) { array_set(s.data, vec(x, y), 0); }
		}
	}
//	for (int x=0; x<30; x++) {
//		for (int y=0; y<30; y++) {
//			struct vector v = vec(x, y);
//			array_set(p.data, v, array_get(s.data, v));
//			compute ++;
//		}
//	}
	if (show) {
		print_state(s);
		printf("\n");
	}
	fflush(stdout);
	// usleep(500000);
}

void simulate(struct simulation sim, int n, int show) {
	int w = 30;
	int h = 30;
	int shape[2] = {30, 30};
	int prog = 0;
	printf("[");
	for (int i=0; i<n-1; i++) {
		struct state p = sim.states[sim.time-1];
		sim.states[sim.time] = (struct state) {new_array(2, p.data.shape)};
		step(sim.states[sim.time], p, i, show);

		sim.time ++;
		int q = 20 * ((double) i / (double) n);
		if (q > prog) {
			printf("#");
			prog = q;
		}
	}
	printf("]\n");
}

int main() {
	printf("ca.c loaded successfully \n");
	// Set random seed
	srand(time(NULL));
	
	compute = 0;

	char input[200];
	fgets(input, 200, stdin);
	char* token = strtok(input, " ");
	char* command;
	char* option;

	struct state state_selection;
	struct state* stateset_selection;

	struct simulation sim_selection;
	char* selection_type;

	char* opt;
	int opt_shape[2] = {30, 30};
	int opt_num = 1;
	int opt_iterations = 100;
	int opt_print = 0;

	int complete = 0;

	// Handle command input
	printf("Processing command...\n");
	do {
		if (token != NULL) {
			token[strcspn(token, "\n")] = 0;
		}
		printf("Handling token [%s] \n", token);
		if (token == NULL || strcmp(token, ">") == 0) {
			if (command == NULL) {
				printf("No command set \n");
			}

			if (strcmp(command, "randomstate") == 0) {
				printf("Generating random state \n");
				if (opt_num == 1) {
					state_selection = random_state(opt_shape);
					selection_type = "state";
				}
				else {
					stateset_selection = calloc(opt_num, sizeof(struct state));
					for (int j=0; j<opt_num; j++) {
						stateset_selection[j] = random_state(opt_shape);
					}
					selection_type = "state_set";
				}
			}
			else if (strcmp(command, "write") == 0) {
				printf("Writing to output file [%s] \n", opt);
				FILE* outfile = fopen(opt, "w");
				if (strcmp(selection_type, "state") == 0) {
					char* summary = state_summary(state_selection);
					fprintf(outfile, "%s", summary);
					free(summary);
				}
				fclose(outfile);
				complete = 1;
			}
			else if (strcmp(command, "simulate") == 0) {
				printf("Executing simulation \n");
				if (strcmp(selection_type, "state") == 0) {
					sim_selection = new_simulation(state_selection, opt_iterations);
					selection_type = "simulation";
					simulate(sim_selection, opt_iterations, opt_print);
				}
				complete = 1;
			}
			else {
				printf("Command not recognized \n");
				complete = 1;
				exit(1);
			}
		}
		else if (strcmp(token, "randomstate") == 0 || strcmp(token, "write") == 0 || strcmp(token, "simulate") == 0) {
			command = strdup(token);
			command[strcspn(command, "\n")] = 0;
			printf("Found command %s \n", command);
		}
		else {
			printf("Found unlabeled option \n");
			opt = strdup(token);
			opt[strcspn(opt, "\n")] = 0;
		}
		
		// if (strcmp(command, "randomstate") == 0) {
			
		token = strtok(NULL, " ");
		usleep(500000);
	} while (!complete);
}
