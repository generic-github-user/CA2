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

#include "array.h"
// #include "vector.h"

// Based on code from https://stackoverflow.com/a/3219471
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

char* COLOR_ORDER[6] = { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA };


// TODO: provide tools for mutating patterns
// TODO: log files
// TODO: add support for long-term experiment databases
// TODO: add function for backtracing cellular automata states
// TODO: add array views & semi-mutable data structures
// TODO: use ndarray structs to store arbitrary collections of states/simulations?

// are nested array structs viable?

// Create a statically typed function that reduces an array to a single value
#define ARRAY_REDUCE(name,type,op,init) type name(array a) { \
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

#define ARRAY_OP(name,op) array name(array a, array b) {\
	array output = new_array(a.rank, a.shape);\
	for (int i=0; i<a.size; i++) {\
		output.data[i] = a.data[i] op b.data[i];\
	}\
	return output;\
}

// TODO
#define EXTRACT(name,property) array extract_##property(state* states, int n) {\
	int* shape = malloc(sizeof(int));\
	shape[0] = n;\
	\
	array output = new_array(1, shape);\
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



struct rule {
	char* name;
};
typedef struct rule rule;

struct neighborhood {
	int inner_radius;
	int outer_radius;
};
typedef struct neighborhood neighborhood;

struct range {

};
typedef struct range range;

// A rule that defines only the conditions that cause cells to "die", be "born", or continue living
struct subtotalistic {
	neighborhood* N;
	int* conditions;
};
typedef struct subtotalistic subtotalistic;

subtotalistic random_subtotal() {

}


struct session {

};
typedef struct session session;


struct totalistic {
	neighborhood* N;
	int** values;
};
typedef struct totalistic totalistic;

// struct name_group
// struct lattice

// TODO: add support for more exotic surfaces (like spheres)
struct manifold {
	char* lattice;
	int dimensions;
	int* shape;

	// Edge behaviors are described as sequential pairs of characters indicating the two directions of motion along each axis, with the following representation:
	// e: expand boundaries to fit the automaton's activity
	// w: wrap from one edge to the opposite (e.g., global wrapping on a 2D plane generates a torus)
	// i: wrap edges, inverted (as with a Mobius strip)
	//
	char edges;
};
typedef struct manifold manifold;

// TODO: type description classes
manifold random_manifold() {
	char edge_types[3] = "ewi";
	int dim = rand() % 4;
//	int* shape = random_array
	int* shape = calloc(dim, sizeof(int));
	for (int i=0; i<dim; i++) {
		shape[i] = (rand() % 95) + 5;
	}
	manifold m = {"grid", dim, shape, edge_types[rand() % 3]};
	return m;
}

// manifold mutate_manifold(manifold m) {



void array_summary(array* a) {
	fprintf(stdout, "Array {min: %i, max: %i}", array_min(a), array_max(a));
}

// filter coordinate list?

// A (relatively) tidy iterative solution using an array of counters is also possible

//void slice_helper(array source, array target, array T, array U,
//		array shape, array count, int i, int n) {
//	if (i < n-1) {
//		for (int j=T.data[i]; j<U.data[i]; j++) {
//			slice_helper(source, target, T, U, i+1, n);
//		}
//	}
//	else {
//		for (int j=0; j<shape[i]; j++) {
//			array_set(target, count, array_get(source,
//			count.data[j] ++;
//		}
//	}
//}

array copy_array(array a) {
	array copy = new_array(a.rank, a.shape);
	for (int i=0; i<a.size; i++) {
		copy.data[i] = a.data[i];
	}
	return copy;
}

vector array_to_vec(array a) {
	return vec(a.data[0], a.data[1], a.data[2]);
}

array array_slice(array a, array T, array U, int rank) {
	array shape = array_bdiff(U, T);
	array slice = new_array(rank, shape.data);

	//int* s = malloc(sizeof(int));
	array counta = new_array(1, &rank);
	array countb = copy_array(T);
//	slice_helper(a, slice, T, U, shape, count, 0, rank);
	int i=0;
	//int j=0;
	// are two counters/an inner while-loop needed?
	while (i < rank) {
		if (counta.data[i] < shape.data[i]) {
			counta.data[i] ++; countb.data[i] ++;
			array_set(
				slice,
				array_to_vec(counta),
				array_get(a, array_to_vec(countb))
			);
		} else {
			counta.data[i] = 0;
			countb.data[i] = T.data[i];
			for (int j=0; j<rank; j++) {
				if (counta.data[j] < shape.data[i]) {
					counta.data[j] ++; countb.data[j] ++;
					break;
				}
			}
		}
	}
	return slice;
}


typedef struct simulation simulation;


// Based on djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned int *values, int n) {
        unsigned long int hash = 5381;

        for (int i=0; i<n; i++) {
            hash = ((hash << 5) + hash) + values[i]; /* hash * 33 + c */
	  }

        return hash;
}

unsigned long int hash_state(state s) {
//	for
//	return hash((unsigned int*) s -> data.data, s -> data.size);
	return hash((unsigned int*) s.data.data, s.data.size);
}

// multidimensional array-based hashtable?
// other open addressing schemes?
// nesting hash tables?

struct hashtable {
	unsigned int size;
	// should we use a double pointer here?
	state** data;
};
typedef struct hashtable hashtable;

hashtable new_hashtable(int size) {
	hashtable h = {size};
	for (int i=0; i<size; i++) {
		h.data[i] = NULL;
	}
	return h;
}

state* lookup(hashtable h, state s) {
	int index = hash_state(s) % h.size;
	for (int i=0; i<h.size; i++) {
		state* value = h.data[index + i];
		if (value != NULL) {
			return value;
		}
	}
	return NULL;
}

void insert(hashtable h, state* s) {
	int index = hash_state(*s) % h.size;
	for (int i=0; i<h.size; i++) {
		state* value = h.data[index + i];
		if (value == NULL) {
			h.data[index + 1] = s;
		}
	}
}

//void remove(hashtable h, state* s) {

//}

//struct list {

state new_state(array data, simulation* sim) {
	state s = {data, 0, 0, sim};
	return s;
}

// Generate geometrically defined chunks from a state
state* extract_slices(state* s, int limit) {
	// TODO
	state* slices = calloc(limit, sizeof(state));
	int q[1] = {2};
	array T = new_array(1, q);
	array U = new_array(1, q);
	int i=0;
	for (int w=1; w<=4; w++) {
		for (int x=0; x<s->data.shape[0]-w; x++) {
			for (int y=0; y<s->data.shape[1]-w; y++) {
				//T.data = {x, y};
				memcpy(&T, (int[]) {x, y}, sizeof(int));
				memcpy(&U, (int[]) {x+w, y+w}, sizeof(int));
				slices[i] = new_state(
					array_slice(s -> data, T, U, 2),
					s -> sim
				);
				i++;
				if (i >= limit) {
					// please forgive me for my sins
					goto end;
				}
			}
		}
	}
	end: return slices;
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

int streq(char* a, char* b) {
	return strcmp(a, b) == 0;
}


struct image {
	array* data;
};
typedef struct image image;

void fill_slice(array* a, vector j, vector k, int value) {
	for (int x=j.x; x<k.x; x++) {
		for (int y=j.y; y<k.y; y++) {
			if (k.x < a -> shape[0] && k.y < a -> shape[1]) {
				array_set(*a, vec(x, y, 0), value);
			}
		}
	}
}

image generate_image(state s, char* color) {
//	int shape[3] = {100, 100, 3};
	int* shape = calloc(3, sizeof(int));
	shape = (int[]) {100, 100, 3};
	array* A = malloc(sizeof(array));
	*A = new_array(3, shape);
	image result = { A };
	fill_array(*result.data, 255);
	int w = 3;
	int h = 3;
	int v[1] = {3};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array cell_color = new_array(1, v);
			if (streq(color, "age")) {

			}
			if (array_get(s.data, vec(x, y, 0)) != 0) {
//				fill_slice(&s.data, vec(x*w, y*h), vec(x*w+w, y*h+h), 0);
				fill_slice(result.data, vec(x*w, y*h, 0), vec(x*w+w, y*h+h, 3), 0);
			}
		}
	}
	return result;
}

void write_image(state s, char* color) {
	// Based on code from https://www.nayuki.io/res/tiny-png-output/MandelbrotPng.c
	printx(3, "Opening image file\n");
	FILE* target = fopen("test.png", "wb");
	if (target == NULL) {
		perror("Error: fopen");
		exit(1);
	}

	printx(3, "Generating image array");
	image image_data = generate_image(s, color);
	// Why does this modify the array's shape (or cause a segfault)?
	// array_summary(image_data.data);

	struct TinyPngOut pngout;
	static const int width = 100;
	static const int height = 100;
	enum TinyPngOut_Status status = TinyPngOut_init(&pngout, (uint32_t)width, (uint32_t)height, target);
	if (status != TINYPNGOUT_OK) {
		printx(3, "Error in TinyPngOut; exiting");
		exit(1);
	}
	printx(3, "Writing pixels to image\n");

	uint8_t *line = calloc((size_t)width * 3, sizeof(uint8_t));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
//			int pixel = image_data.data -> data[(x*100+y)*3];
			for (int z = 0; z < 3; z++) {
				int pixel = array_get(*image_data.data, vec(x, y, z));
				line[x*3+z] = pixel;
			}
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
	array ages;
	// Amount of space dynamically alloated for members of this struct
	int size;
};

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


char* microplot(simulation s) {
	char* result = calloc(s.steps, sizeof(char));
	//int min = array_min(extract_population(s.states, s.steps));
	//int max = array_max(extract_population(s.states, s.steps));
	char plotsymbols[5] = " .*oO";
	for (int i=0; i<s.steps; i++) {
	//	result[i] = plotsymbols[(int) ((double) (s.states[i].population - min) / (double) (max - min) * 5)];
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
				if (inrange(c, 0, source.shape[0]-1) && inrange(d, 0, source.shape[1]-1)) {
					neighbors += array_get(source.data, vec(c, d, 0));
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
	for (int x=0; x<s.shape[0]; x++) {
		for (int y=0; y<s.shape[1]; y++) {
			neighbors = count_neighbors(s, x, y, cc);
			array_set(counts.data, vec(x, y, 0), neighbors);
			//(*cc) ++;
		}
	}
	return counts;
}



//delta

void step(state* s, state* p, int i, int show, int* cc, simulation sim, int unicode, char color) {
	if (show) {
		printf("Simulating frame %i \n", i+1);
	}
	int neighbors = 0;
	for (int x=0; x<s->shape[0]; x++) {
		for (int y=0; y<s->shape[1]; y++) {
			array_set(s -> data, vec(x, y, 0), array_get(p -> data, vec(x, y, 0)));
			sim.ages.data[get_coord(sim.ages, vec(x, y, 0))] ++;
			compute ++;
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

//	for (int x=0; x<30; x++) {
//		for (int y=0; y<30; y++) {
//			vector v = vec(x, y, 0);
//			array_set(p.data, v, array_get(s.data, v));
//			compute ++;
//		}
//	}
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
void simulate(simulation* sim, int n, int show, int level, int unicode, char color) {
	int prog = 0;
	printx(level+1, "");
	printf("Simulating %i iterations\n", n-1);
	printx(level+1, "");
	printf("[");
	for (int i=0; i<n-1; i++) {
		state* p = &(sim->states)[(sim->time)-1];
		(sim -> states)[sim -> time] = (state) {new_array(2, p -> data.shape), 0, 0, sim};
		step(&(sim->states)[sim->time], p, i, show, &(sim -> compute), *sim, unicode, color);

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


int iscommand(char* text) {
	char* commands[9] = {
		"randomstate", "enumerate",
		"simulate",
		"collapse", "min", "max",
		"write", "print", "render"
	};
	for (int i=0; i<9; i++) {
		if (strcmp(text, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
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
	int opt_unicode = 1;
	char* opt_color = "age";

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
					write_image(state_selection, opt_color);
				}
			}
			else if (streq(command, "simulate")) {
				printx(2, "Executing simulation");
				if (streq(selection_type, "state")) {
					sim_selection = new_simulation(state_selection, opt_iterations);
					selection_type = "simulation";
					simulate(&sim_selection, opt_iterations, opt_print, 2, opt_unicode, opt_color[0]);
				}
				else if (streq(selection_type, "state_set")) {
					simset_selection = calloc(opt_num, sizeof(simulation));
					for (int j=0; j<opt_num; j++) {
						simset_selection[j] = new_simulation(stateset_selection[j], opt_iterations);
						simulate(&simset_selection[j], opt_iterations, opt_print, 2, opt_unicode, opt_color[0]);
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
				case 'u': { opt_unicode = 1; break; }
				case 'c': { opt_color = token; break; }
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
