#include "mainheaders.h"

// TODO: provide tools for mutating patterns
// TODO: log files
// TODO: add support for long-term experiment databases
// TODO: add function for backtracing cellular automata states
// TODO: add array views & semi-mutable data structures
// TODO: use ndarray structs to store arbitrary collections of states/simulations?
// TODO: add Collection type?
// TODO: are nested array structs viable?

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

struct range {

};
typedef struct range range;

// A rule that defines only the conditions that cause cells to "die", be "born", or continue living (the best-known example probably being Conway's Game of Life)
// [nonstandard]
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

session new_session() {

}

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



int streq(char* a, char* b) {
	return strcmp(a, b) == 0;
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
