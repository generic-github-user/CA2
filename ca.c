#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#include <inttypes.h>
#include "spng.h"
#include "TinyPngOut.h"

// TODO: provide tools for mutating patterns
// TODO: log files
// TODO: add support for long-term experiment databases
// TODO: add function for backtracing cellular automata states
// TODO: add array views & semi-mutable data structures

// Create a statically typed function that reduces an array to a single value
#define ARRAY_REDUCE(name,type,op,init) type name(struct array a) { \
	type output = init;\
	for (int i=0; i<a.size; i++) {\
		output = output op a.data[i];\
	}\
	return output;\
}

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

#define ARRAY_OP(name,op) struct array name(struct array a, struct array b) {\
	struct array output = new_array(a.rank, a.shape);\
	for (int i=0; i<a.size; i++) {\
		output.data[i] = a.data[i] op b.data[i];\
	}\
	return output;\
}

// TODO
#define EXTRACT(name,property) struct array extract_##property(state* states, int n) {\
	int* shape = malloc(sizeof(int));\
	shape[0] = n;\
	\
	struct array output = new_array(1, shape);\
	for (int i=0; i<n; i++) {\
		output.data[i] = states[i].property;\
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
FILE* logfile;

// Clip an integer to a range (modify in-place)
int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

// A vector or coordinate
struct vector {
	int x, y;
};
typedef struct vector vector;

vector vec(int x, int y) {
	return (vector) { x, y };
}

struct rule {
	char* name;
};

struct neighborhood {
	int inner_radius;
	int outer_radius;
};

struct range {

};

// A rule that defines only the conditions that cause cells to "die", be "born", or continue living
struct subtotalistic {
	struct neighborhood* N;
	int* conditions;
};

struct subtotalistic random_subtotal() {

}

struct totalistic {
	struct neighborhood* N;
	int** values;
};

// struct name_group
// struct lattice

struct manifold {
	char* lattice;
	int dimensions;
	int shape;

	// Edge behaviors are described as sequential pairs of characters indicating the two directions of motion along each axis, with the following representation:
	// e: expand boundaries to fit the automaton's activity
	// w: wrap from one edge to the opposite (e.g., global wrapping on a 2D plane generates a torus)
	// i: wrap edges, inverted (as with a Mobius strip)
	//
	char* edges;
};

// TODO: type description classes
struct manifold random_manifold() {
	struct manifold m = {};
	return m;
}



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
	int* data = calloc(size, sizeof(int));
	struct array a = { rank, shape, size, data };
	fill_array(a, 0);
	return a;
};

// Convert a series of indices to a corresponding memory address in the internal representation of the array data
int get_coord(struct array a, vector z) {
	// ?
	return z.x * a.shape[1] + z.y;
}

int array_get(struct array a, vector z) {
	return a.data[get_coord(a, z)];
}

void array_set(struct array a, vector z, int value) {
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
	return (double) array_sum(a) / (double) a.size;
}

int array_min(struct array a) {
	int output = a.data[0];
	for (int i=0; i<a.size; i++) {
		if (a.data[i] < output) {
			output = a.data[i];
		}
	}
	return output;
}

int array_max(struct array a) {
	int output = a.data[0];
	for (int i=0; i<a.size; i++) {
		if (a.data[i] > output) {
			output = a.data[i];
		}
	}
	return output;
}

ARRAY_OP(array_bsum, +);
ARRAY_OP(array_bdiff, -);
ARRAY_OP(array_bprod, *);
ARRAY_OP(array_bdiv, /);
ARRAY_OP(array_bmod, %);

void array_summary(struct array a) {
	fprintf(stdout, "Array {min: %i, max: %i}", array_min(a), array_max(a));
}

// A static "frame" of a simulation to which the rules of a cellular automata may be repeatedly applied in a simulation
struct state {
	struct array data;
	int population;
	double density;
};
typedef struct state state;

void update_state(state* s) {
	(s -> population) = array_sum(s -> data);
	// should s -> data be a pointer?
	(s -> density) = (double) array_sum(s -> data) / (double) (s -> data).size;
}

PTR_REDUCE(max_population, population, >);
PTR_REDUCE(min_population, population, <);

EXTRACT(extract_population, population);

//void printx(int level, char* fmt, ...) {
void printx(int level, char* text) {
	for (int i=0; i<level; i++) {
		printf("  ");
	}
	if (strcmp(text, "") != 0) {
		printf("%s \n", text);
		fprintf(logfile, "%s \n", text);
	}
}


struct image {
	struct array* data;
};

void fill_slice(struct array* a, vector j, vector k, int value) {
	for (int x=j.x; x<k.x; x++) {
		for (int y=j.y; y<k.y; y++) {
			if (k.x < a -> shape[0] && k.y < a -> shape[1]) {
				array_set(*a, vec(x, y), value);
			}
		}
	}
}

struct image generate_image(state s) {
	int shape[3] = {100, 100, 3};
	struct array* A = malloc(sizeof(struct array));
	*A = new_array(3, shape);
	struct image result = { A };
	fill_array(*result.data, 255);
	int w = 3;
	int h = 3;
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			if (array_get(s.data, vec(x, y)) != 0) {
//				fill_slice(&s.data, vec(x*w, y*h), vec(x*w+w, y*h+h), 0);
				fill_slice(result.data, vec(x*w, y*h), vec(x*w+w, y*h+h), 0);
			}
		}
	}
	return result;
}

//void write_image(state s) {
//	FILE* target = fopen("test.png", "wb");
//
//	spng_ctx *enc = spng_ctx_new(SPNG_CTX_ENCODER);
//	spng_set_option(enc, SPNG_ENCODE_TO_BUFFER, 1);
//	struct spng_ihdr ihdr =
//	{
//	    .width = 100,
//	    .height = 100,
//	    .bit_depth = 8,
//	    .color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA
//	};
//	spng_set_ihdr(enc, &ihdr);
//	//size_t png_size[2] = {100, 100};
//	size_t png_size = 100;
//	struct image image_data = generate_image(s);
//	spng_encode_image(enc, image_data.data.data, &png_size, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
//	long error;
//	//void *png = spng_get_png_buffer(enc, &png_size, &error);
//	void *png = target;
//
//	free(png);
//	spng_ctx_free(enc);
//	free(image_data.data.data);
//}

void write_image(state s) {
	// Based on code from https://www.nayuki.io/res/tiny-png-output/MandelbrotPng.c
	printx(3, "Opening image file\n");
	FILE* target = fopen("test.png", "wb");
	if (target == NULL) {
		perror("Error: fopen");
		exit(1);
	}

	printx(3, "Generating image array");
	struct image image_data = generate_image(s);
	array_summary(*image_data.data);
	
	struct TinyPngOut pngout;
	static const int width = 100;
	static const int height = 100;
	enum TinyPngOut_Status status = TinyPngOut_init(&pngout, (uint32_t)width, (uint32_t)height, target);
	if (status != TINYPNGOUT_OK) {
		exit(1);
	}
	printx(3, "Writing pixels to image\n");

	uint8_t *line = calloc((size_t)width * 3, sizeof(uint8_t));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
//			int* pixel = array_get(image_data.data, vec(x, y));
//			line[x * 3 + 0] = (array_get(image_data.data, vec(x, y)));
//			line[x * 3 + 1] = (array_get(image_data.data, vec(x, y)));
//			line[x * 3 + 2] = (array_get(image_data.data, vec(x, y)));
			
			int pixel = image_data.data -> data[x*100+y];
			line[x*3+0] = pixel;
			line[x*3+1] = pixel;
			line[x*3+2] = pixel;
		}
		status = TinyPngOut_write(&pngout, line, (size_t)width);
		if (status != TINYPNGOUT_OK) {
			printx(3, "Encountered error while writing image; exiting");
			exit(1);
		}
	}
	printf("%s", " -- Done");

	free(line);
	free(image_data.data -> data);
}

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {
	state* states;
	int time, steps, compute;
};
typedef struct simulation simulation;

// should s be a state pointer?
simulation new_simulation(state s, int steps) {
	simulation sim;
	sim.states = (state*) calloc(steps, sizeof(state));
	sim.states[0] = s;

	sim.time = 1;
	sim.steps = steps;
	sim.compute = 0;
//
//	printf("Created new simulation
	return sim;
}

// Generate a random state
state random_state(int* shape) {
	state result = {new_array(2, shape)};
	for (int x=0; x<shape[0]; x++) {
		for (int y=0; y<shape[1]; y++) {
			array_set(result.data, vec(x, y), rand() % 2);
			compute ++;
		}
	}
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
			char c = array_get(s.data, vec(x, y)) ? '#' : ' ';
			output[i++] = c;
		}
		output[i++] = '\n';
	}
	output[i++] = '\0';
	return output;
}

char* microplot(simulation s) {
	char* result = calloc(s.steps, sizeof(char));
	int min = array_min(extract_population(s.states, s.steps));
	int max = array_max(extract_population(s.states, s.steps));
	char plotsymbols[5] = " .*oO";
	for (int i=0; i<s.steps; i++) {
		result[i] = plotsymbols[(int) ((double) (s.states[i].population - min) / (double) (max - min) * 5)];
	}
	return result;
}

void sim_summary(simulation* s) {
	fprintf(stdout, "Simulation (%i steps) \n\n", s -> steps);
	fprintf(stdout, "Population -- %s\n", microplot(*s));
	for (int i=0; i<s -> steps; i++) {
		fprintf(stdout, "  Step %i: %i \n", i, (s -> states)[i].population);
	}
}

char* sim_info(simulation s) {
	char* result = calloc(50, sizeof(char));
	strcat(result, "Simulation { Length");
//	strcat(result, itoa(s.steps));
	return result;
}

int inrange(int x, int n, int m) {
	return x >= n && x <= m;
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
				if (inrange(c, 0, 29) && inrange(d, 0, 29)) {
					neighbors += array_get(source.data, vec(c, d));
				}
			}
			compute ++;
			(*cc) ++;
		}
	}
	return neighbors;
}

state map_neighbors(state s, int* cc) {
	int neighbors;
	state counts = {new_array(2, s.data.shape)};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			neighbors = count_neighbors(s, x, y, cc);
			array_set(counts.data, vec(x, y), neighbors);
			//(*cc) ++;
		}
	}
	return counts;
}


void print_state(state s) {
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

state* clone_state(state s) {
	//state clone = (state) {new_array(2, s.data.shape)};
	state* clone = malloc(sizeof(state));
	*clone = (state) {new_array(2, s.data.shape)};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array_set(clone -> data, vec(x, y), array_get(s.data, vec(x, y)));
			compute ++;
		}
	}
	return clone;
}

void step(state* s, state* p, int i, int show, int* cc) {
	if (show) {
		printf("Simulating frame %i \n", i+1);
	}
	int neighbors = 0;
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array_set(s -> data, vec(x, y), array_get(p -> data, vec(x, y)));
			compute ++;
			(*cc) ++;
		}
	}

	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			neighbors = count_neighbors(*p, x, y, cc);
			(*cc) ++;
			if (neighbors == 3) { array_set(s -> data, vec(x, y), 1); }
			if (neighbors < 2 || neighbors > 3) { array_set(s -> data, vec(x, y), 0); }
		}
	}
	update_state(s);

//	for (int x=0; x<30; x++) {
//		for (int y=0; y<30; y++) {
//			vector v = vec(x, y);
//			array_set(p.data, v, array_get(s.data, v));
//			compute ++;
//		}
//	}
	if (show) {
		print_state(*s);
		printf("\n");
	}
	fflush(stdout);
	if (show) {
		usleep(200000);
	}
}



// note: don't pass by value?!?!
void simulate(simulation* sim, int n, int show, int level) {
	int prog = 0;
	printx(level+1, "");
	printf("Simulating %i iterations\n", n-1);
	printx(level+1, "");
	printf("[");
	for (int i=0; i<n-1; i++) {
		state* p = &(sim->states)[(sim->time)-1];
		(sim -> states)[sim -> time] = (state) {new_array(2, p -> data.shape)};
		step(&(sim->states)[sim->time], p, i, show, &(sim -> compute));

		(sim -> time) ++;
		int q = 40 * ((double) i / (double) n);
		if (q > prog) {
			printf("#");
			prog = q;
		}
		(sim -> compute) ++;
	}
	printf("]\n");

	char temp[50];
	sprintf(temp, "Simulation complete; compute usage was %i \n", sim -> compute);
	printx(level+1, temp);
//	free(temp);
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

int iscommand(char* text) {
	char* commands[9] = {"randomstate", "write", "simulate", "collapse", "min", "max", "enumerate", "print", "render"};
	for (int i=0; i<9; i++) {
		if (strcmp(text, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int streq(char* a, char* b) {
	return strcmp(a, b) == 0;
}

int states_equal(state a, state b) {
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			if (array_get(a.data, vec(x, y)) != array_get(b.data, vec(x, y))) {
				return 0;
			}
		}
	}
	return 1;
}

void process_command(char* cmd, FILE* log) {
	char* token = strtok(cmd, " ");
	char* command;
	char* option;
	char optionc = '\0';

	state state_selection;
	state* stateset_selection;

	simulation sim_selection;
	simulation* simset_selection;
	
	char* selection_type;

	char* opt;
	int opt_shape[2] = {30, 30};
	int opt_num = 1;
	int opt_iterations = 100;
	int opt_print = 0;

	int complete = 0;

	// Handle command input
	printx(0, "Processing command...");
	do {
		if (token != NULL) {
			token[strcspn(token, "\n")] = 0;
		}
		printx(1, "");
		printf("Handling token [%s] \n", token);

		if (token == NULL) {
			if (optionc == 'p') {
				opt_print = 1;
			}
		}

		if (token == NULL || streq(token, ">")) {
			if (command == NULL) {
				printx(2, "No command set");
			}

			if (streq(command, "randomstate")) {
				printx(2, "Generating random state");
				if (opt_num == 1) {
					state_selection = random_state(opt_shape);
					selection_type = "state";
			}
				else {
					stateset_selection = calloc(opt_num, sizeof(state));
					for (int j=0; j<opt_num; j++) {
						stateset_selection[j] = random_state(opt_shape);
					}
					selection_type = "state_set";
				}
			}
			// TODO: exploit symmetries and pattern components for more compact storage/representation?
			else if (streq(command, "enumerate")) {
				printx(2, "Enumerating states...");
				int i = 1;
				int z = 0;
				// TODO
				//if (stateset_selection == NULL) {
				stateset_selection = calloc(opt_num, sizeof(state));
				//}
				stateset_selection[0] = (state) {new_array(2, opt_shape)};
				// TODO: update stats?
				state* s;
				while (i < opt_num) {
					printx(3, "");
					printf("Generating state %i\n", i);

					z = 0;
					s = &stateset_selection[i];
					stateset_selection[i] = *clone_state(stateset_selection[i-1]);
					while ((s->data).data[z] == 1) {
						(s->data).data[z] = 0;
						z ++;
						if (z >= (s->data).size) {
							break;
						}
					}
					(s->data).data[z] = 1;
					i ++;
				}
				selection_type = "state_set";
			}
			else if (streq(command, "write")) {
				printx(2, "");
				printf("Writing to output file [%s] \n", opt);
				FILE* outfile = fopen(opt, "w");
				if (strcmp(selection_type, "state") == 0) {
					write_state(state_selection, outfile);
					//free(state_selection);
				}
				else if (strcmp(selection_type, "state_set") == 0) {
					printx(3, "");
					printf("Writing %i states \n", opt_num);
					for (int j=0; j<opt_num; j++) {
						write_state(stateset_selection[j], outfile);
						fprintf(outfile, "\n\n");
					}
					free(stateset_selection);
				}
				printx(3, "Closing output file");
				fclose(outfile);
				complete = 1;
			}
			else if (streq(command, "print")) {
				if (streq(selection_type, "simulation")) {
					sim_summary(&sim_selection);
				}
				else if (streq(selection_type, "simulation_set")) {
					for (int j=0; j<opt_num; j++) {
						sim_summary(&simset_selection[j]);
					}
				}
			}
			else if (streq(command, "render")) {
				printx(2, "Rendering selected state to image...");
				if (streq(selection_type, "state")) {
					write_image(state_selection);
				}
			}
			else if (streq(command, "simulate")) {
				printx(2, "Executing simulation");
				if (streq(selection_type, "state")) {
					sim_selection = new_simulation(state_selection, opt_iterations);
					selection_type = "simulation";
					simulate(&sim_selection, opt_iterations, opt_print, 2);
				}
				else if (streq(selection_type, "state_set")) {
					simset_selection = calloc(opt_num, sizeof(simulation));
					for (int j=0; j<opt_num; j++) {
						simset_selection[j] = new_simulation(stateset_selection[j], opt_iterations);
						simulate(&simset_selection[j], opt_iterations, opt_print, 2);
					}
					selection_type = "simulation_set";
				}
			}
			else if (streq(command, "collapse")) {
				printx(2, "Collapsing simulation(s)");
				if (streq(selection_type, "simulation_set")) {
					stateset_selection = calloc(opt_num, sizeof(state));
					for (int j=0; j<opt_num; j++) {
						//simulation sim = simset_selection[j];
						simulation* sim = &simset_selection[j];
						printx(2, "");
						printf("Getting state at index %i\n", sim -> time);
						stateset_selection[j] = sim -> states[(sim -> time) - 2];
					}
					selection_type = "state_set";
				}
				else {
					printx(2, "Command not supported for this data type");
				}
			}
			else if (streq(command, "min")) {
				printx(2, "Reducing to minimum");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						state_selection = min_population(stateset_selection, opt_num);
					}
				}
			}
			else if (streq(command, "max")) {
				printx(2, "Reducing to maximum");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						state_selection = max_population(stateset_selection, opt_num);
					}
				}
			}
			else {
				printx(2, "Command not recognized");
				complete = 1;
				exit(1);
			}

			if (token == NULL) {
				complete = 1;
			}
		}
		else if (iscommand(token)) {
			command = strdup(token);
			command[strcspn(command, "\n")] = 0;
			printx(2, "");
			printf("Found command %s \n", command);
		}
		else if (token[0] == '-') {
			optionc = token[1];
		}
		else if (optionc != '\0') {
			printx(2, "");
			printf("Received option [-%c] with value %s \n", optionc, token);
			switch (optionc) {
				case 'n': {
					printx(2, "");
					opt_num = atoi(token);
					optionc = '\0';
					break;
				}
				case 'i': {
					opt_iterations = atoi(token);
					optionc = '\0';
					break;
				}
				case 'p': {
					opt_print = 1;
					break;
				}
			}
		}
		else {
			printx(2, "Found unlabeled option");
			opt = (char*) strdup(token);
			opt[strcspn(opt, "\n")] = 0;
		}
		


		token = strtok(NULL, " ");
		usleep(200000);
	} while (!complete);
}



int main() {
	printf("ca.c loaded successfully \n");
	// Set random seed
	srand(time(NULL));
	
	compute = 0;
	logfile = fopen("ca_log.txt", "a");


	char input[200];
	fgets(input, 200, stdin);
	if (streq(input, "test1\n")) {
		char* r = "randomstate -n 20 > simulate -i 200 > collapse > write test1.txt";
		process_command(r, logfile);
	}
	else {
		process_command(input, logfile);
	}
	fclose(logfile);
}
