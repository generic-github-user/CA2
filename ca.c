/* Generated from ca.c0 at 06/04/2022 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include "mainheaders.h"



const char* COLOR_ORDER[6] = { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA };
FILE* logfile = NULL;
int verbosity = 6;

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
				slices[i] = *new_state(
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

// Start "interactive mode", a REPL-style command shell in which the user can enter commands to create, view, and manipulate objects
void interactive() {
	printf("ca.c loaded successfully \n");
	printf("Entering interactive mode \n");

	// Example commands that can be run with "ex#"
	char* ex[] = {
		"randomstate -n 20 > simulate -i 200 > collapse > write test1.txt",
		"randomstate > simulate > get population > plot",
		"randomstate > simulate > reduce sum > plot"
	};

	// Set random seed
	srand(time(NULL));

	compute = 0;
	logfile = fopen("ca_log.txt", "a");


	//char input[200];
	// Get command from stdin
	char* input = calloc(200, sizeof(char));
	fgets(input, 200, stdin);
	if (strncmp("ex", input, 2) == 0) {
		strcpy(input, ex[atoi(input+2)]);
		process_command(input, logfile);
	} else {
		process_command(input, logfile);
	}
	fclose(logfile);
}

int main(int argc, char** argv) {
	static struct option long_options[] = {
		{"about", no_argument, 0, 0}
	};
	char* optvalue = NULL;
	char opt;
	int option_index = 0;

	// Loop through command-line arguments
	while ((opt = getopt_long(argc, argv, "ic:hv:", long_options, &option_index)) != -1 ) {
		switch (opt) {
			case 'i': {
				interactive();
				break;
			}
			case 'c': {
//				process_command(optarg, NULL);
				optvalue = optarg;
//				optvalue = argopt;
				process_command(optvalue, NULL);
				break;
			}
			case 'h': {
				printx(0, "Documentation of available commands can be found in the project README: https://github.com/generic-github-user/CA2/blob/master/README.md. The file also has subsections with more information about command options, types, and the structure of CA2. \n");
				break;
			}
			case 'v': {
				verbosity = atoi(optarg);
				break;
			}

			case 0: {
				printx(0, "CA2, under development from 2022 to ?. GitHub repository is https://github.com/generic-github-user/CA2. \n");
				break;
			}

			default: {
				exit(1);
			}
		}
	}
}
