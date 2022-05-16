#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

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

#define PTR_REDUCE(name,property,op) struct state name(struct state* states, int n) {\
	struct state output = states[0];\
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

struct vector vec(int x, int y) {
	return (struct vector) { x, y };
}

// struct name_group

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
	int* data = calloc(size, sizeof(int));
	struct array a = { rank, shape, size, data };
	fill_array(a, 0);
	return a;
};

// Convert a series of indices to a corresponding memory address in the internal representation of the array data
int get_coord(struct array a, struct vector z) {
	// ?
	return z.x * a.shape[1] + z.y;
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
	return (double) array_sum(a) / (double) a.size;
}


ARRAY_OP(array_bsum, +);
ARRAY_OP(array_bdiff, -);
ARRAY_OP(array_bprod, *);
ARRAY_OP(array_bdiv, /);
ARRAY_OP(array_bmod, %);



// A static "frame" of a simulation to which the rules of a cellular automata may be repeatedly applied in a simulation
struct state {
	struct array data;
	int population;
	double density;
};

void update_state(struct state* s) {
	(s -> population) = array_sum(s -> data);
	// should s -> data be a pointer?
	(s -> density) = (double) array_sum(s -> data) / (double) (s -> data).size;
}

PTR_REDUCE(max_population, population, >);
PTR_REDUCE(min_population, population, <);

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {
	struct state* states;
	int time, steps, compute;
};

// should s be a state pointer?
struct simulation new_simulation(struct state s, int steps) {
	struct simulation sim;
	sim.states = (struct state*) calloc(steps, sizeof(struct state));
	sim.states[0] = s;

	sim.time = 1;
	sim.steps = steps;
	sim.compute = 0;
//
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
	update_state(&result);
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
int count_neighbors(struct state source, int x, int y, int* cc) {
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

struct state map_neighbors(struct state s, int* cc) {
	int neighbors;
	struct state counts = {new_array(2, s.data.shape)};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			neighbors = count_neighbors(s, x, y, cc);
			array_set(counts.data, vec(x, y), neighbors);
			//(*cc) ++;
		}
	}
	return counts;
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

struct state* clone_state(struct state s) {
	//struct state clone = (struct state) {new_array(2, s.data.shape)};
	struct state* clone = malloc(sizeof(struct state));
	*clone = (struct state) {new_array(2, s.data.shape)};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array_set(clone -> data, vec(x, y), array_get(s.data, vec(x, y)));
			compute ++;
		}
	}
	return clone;
}

void step(struct state* s, struct state* p, int i, int show, int* cc) {
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
//			struct vector v = vec(x, y);
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

// note: don't pass by value?!?!
void simulate(struct simulation* sim, int n, int show, int level) {
	int prog = 0;
	printx(level+1, "");
	printf("Simulating %i iterations\n", n-1);
	printx(level+1, "");
	printf("[");
	for (int i=0; i<n-1; i++) {
		struct state* p = &(sim->states)[(sim->time)-1];
		(sim -> states)[sim -> time] = (struct state) {new_array(2, p -> data.shape)};
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



void write_state(struct state s, FILE* fptr) {
	fprintf(fptr, "Population: %i \n", array_sum(s.data));
	fprintf(fptr, "Density: %f \n", (double) array_sum(s.data) / (double) s.data.size);
	int cc = 0;
	fprintf(fptr, "Avg. neighbors: %f \n", array_mean(map_neighbors(s, &cc).data));

	char* summary = state_summary(s);
	fprintf(fptr, "%s", summary);
	free(summary);
}

int iscommand(char* text) {
	char* commands[7] = {"randomstate", "write", "simulate", "collapse", "min", "max", "enumerate"};
	for (int i=0; i<7; i++) {
		if (strcmp(text, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int streq(char* a, char* b) {
	return strcmp(a, b) == 0;
}

void process_command(char* cmd, FILE* log) {
	char* token = strtok(cmd, " ");
	char* command;
	char* option;
	char optionc = '\0';

	struct state state_selection;
	struct state* stateset_selection;

	struct simulation sim_selection;
	struct simulation* simset_selection;
	
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
					stateset_selection = calloc(opt_num, sizeof(struct state));
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
				stateset_selection = calloc(opt_num, sizeof(struct state));
				//}
				stateset_selection[0] = (struct state) {new_array(2, opt_shape)};
				struct state* s;
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
				}
				else if (strcmp(selection_type, "state_set") == 0) {
					for (int j=0; j<opt_num; j++) {
						write_state(stateset_selection[j], outfile);
						fprintf(outfile, "\n\n");
					}
				}
				fclose(outfile);
				complete = 1;
			}
			else if (streq(command, "simulate")) {
				printx(2, "Executing simulation");
				if (streq(selection_type, "state")) {
					sim_selection = new_simulation(state_selection, opt_iterations);
					selection_type = "simulation";
					simulate(&sim_selection, opt_iterations, opt_print, 2);
				}
				else if (streq(selection_type, "state_set")) {
					simset_selection = calloc(opt_num, sizeof(struct simulation));
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
					stateset_selection = calloc(opt_num, sizeof(struct state));
					for (int j=0; j<opt_num; j++) {
						//struct simulation sim = simset_selection[j];
						struct simulation* sim = &simset_selection[j];
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
			printf("Received option [-%c] with value %s", optionc, token);
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
