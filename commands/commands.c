/* Generated from ./commands/commands.c0 at 05/25/2022, 02:00:59 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../state/state.h"
#include "../helpers/helpers.h"
#include "../image/image.h"
#include "../plot/plot.h"
#include "../mainheaders.h"

// Check if a string is a valid command name
int iscommand(char* text) {
	char* commands[] = {
		"randomstate", "enumerate",
		"simulate",
		"collapse", "min", "max", "get",
		"write", "print", "render", "table",
		"quit",
		"plot"
	};
	for (int i=0; i<13; i++) {
		if (strcmp(text, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void array_summary(array* a, int level) {
	printx(
		level+1,
		"Array {min: %i, max: %i, size: %i}\n",
		array_min(a), array_max(a), a->size
	);
}

// Execute a command string, writing to stdout and the provided log file
void process_command(char* cmd, FILE* log) {
	char* cmd_src = strdup(cmd);
	char* token = strtok(cmd, " ");
	char* command;
	char* option;
	char optionc = '\0';

	// could this be a single [void] pointer?
	void** selection;
	void** set_selection;

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
	printx(0, "Processing command ("MAGENTA"%s"RESET")...\n", cmd_src);
	do {
		if (token != NULL) {
			token[strcspn(token, "\n")] = 0;
		}
		printx(1, "Handling token "MAGENTA"%s"RESET"\n", token);

		if (token == NULL) {
			if (optionc == 'p') {
				opt_print = 1;
			}
		}

		if (token == NULL || streq(token, ">")) {
			printx(2, "Executing command "MAGENTA"%s"RESET"\n", command);
			if (command == NULL) {
				printx(2, "No command set\n");
			}

			if (streq(command, "randomstate")) {
				/* Imported from ./commands/randomstate_cmd.ct at 05/24/2022, 22:40:57 */ 
printx(2, "Generating random state...\n");
if (opt_num == 1) {
	selection = malloc(1);
	*selection = (state*) random_state(opt_shape);
	selection_type = "state";
	printx(2, "%s\n", state_info(*((state*) *selection)));
}
else {
	selection = malloc(opt_num);
	for (int j=0; j<opt_num; j++) {
		selection[j] = (state*) random_state(opt_shape);
		printx(3, "%s\n", state_info(*((state*) selection[j])));
	}
	selection_type = "state_set";
}


			}
			// TODO: exploit symmetries and pattern components for more compact storage/representation?
			else if (streq(command, "enumerate")) {
				/* Imported from ./commands/enumerate_cmd.ct at 05/24/2022, 22:40:57 */ 
printx(2, "Enumerating states...");
int i = 1;
int z = 0;
// TODO
//if (stateset_selection == NULL) {
selection = calloc(opt_num, sizeof(state));
//}
selection[0] = new_state(new_array(2, opt_shape), NULL);
// TODO: update stats?
state* s;
while (i < opt_num) {
	printx(3, "Generating state %i\n", i);

	z = 0;
	s = selection[i];
	selection[i] = (state*) clone_state(*((state*) selection[i-1]));
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
				/* Imported from ./commands/write_cmd.ct at 05/24/2022, 22:40:57 */ 
printx(2, "Writing to output file [%s] \n", opt);
FILE* outfile = fopen(opt, "w");
if (strcmp(selection_type, "state") == 0) {
	write_state(*((state*) *selection), outfile);
	//free(state_selection);
}
else if (strcmp(selection_type, "state_set") == 0) {
	printx(3, "Writing %i states \n", opt_num);
	for (int j=0; j<opt_num; j++) {
		write_state(*((state*) selection[j]), outfile);
		fprintf(outfile, "\n\n");
	}
	free(selection);
}
printx(3, "Closing output file");
fclose(outfile);
complete = 1;


			}
			else if (streq(command, "print")) {
				/* Imported from ./commands/print_cmd.ct at 05/24/2022, 22:40:57 */ 
if (streq(selection_type, "simulation")) {
	sim_summary((simulation*) selection);
}
else if (streq(selection_type, "simulation_set")) {
	for (int j=0; j<opt_num; j++) {
		sim_summary(selection[j]);
	}
}

			}
			else if (streq(command, "render")) {
				/* Imported from ./commands/render_cmd.ct at 05/24/2022, 22:40:57 */ 
printx(2, "Rendering selected state to image...");
if (streq(selection_type, "state")) {
	write_image(*((state*) selection), opt_color);
}

			}
			else if (streq(command, "table")) {
				if (streq(selection_type, "simulation")) {
					sim_data(*((simulation*) selection));
				}
			}
			else if (streq(command, "simulate")) {
				/* Imported from ./commands/simulate_cmd.ct at 05/24/2022, 22:40:57 */ 
printx(2, "Executing simulation\n");

// TODO: simulate dynamic dispatch
if (streq(selection_type, "state")) {
	// why is dereferencing the selection pointer not an issue?
	*selection = new_simulation(**((state**) selection), opt_iterations);
	selection_type = "simulation";
	simulate(*selection, opt_iterations, opt_print, 2, opt_unicode, opt_color[0], !opt_print);
	// TODO: automatically deallocate strings from heap after printing
	printx(2, "%s\n", sim_info(*((simulation*) *selection)));
}
else if (streq(selection_type, "state_set")) {
	// !!!!
	// selection = malloc(opt_num);
	for (int j=0; j<opt_num; j++) {
		selection[j] = new_simulation(*((state*) selection[j]), opt_iterations);
		simulate(
			(simulation*) selection[j], opt_iterations,
			opt_print, 2, opt_unicode, opt_color[0], !opt_print
		);
	}
	selection_type = "simulation_set";
}


			}
			else if (streq(command, "collapse")) {
				printx(2, "Collapsing simulation(s)\n");
				if (streq(selection_type, "simulation_set")) {
					selection = calloc(opt_num, sizeof(state));
					for (int j=0; j<opt_num; j++) {
						//simulation sim = simset_selection[j];
						simulation* sim = selection[j];
						printx(2, "Getting state at index %i\n", sim -> time);
						// should this account for the size of a state struct?
						selection[j] = (sim -> states) + (sim -> time) - 2;
					}
					selection_type = "state_set";
				}
				else if (streq(selection_type, "simulation")) {
					simulation* sim = *selection;
					*selection = (sim -> states) + (sim -> time) - 2;
				}
				else {
					printx(2, "Command not supported for this data type\n");
				}
			}
			else if (streq(command, "get")) {
				//selection = (void*) malloc(1);
				selection_type = "array";
				if (streq(opt, "population")) {
					// *((array*) selection) = extract_population(*selection, ((simulation*) *selection)->steps);
					array* a = malloc(sizeof(array));
					*a = extract_population((state*) ((simulation*) *selection)->states, ((simulation*) *selection) -> steps);
					array_summary(a, 2);
					// TODO
//					selection = (void*) malloc(1);
					selection = (void*) malloc(sizeof(array));
					selection = a;

				}
			}
			else if (streq(command, "min")) {
				printx(2, "Reducing to minimum\n");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						*selection = min_population(*selection, opt_num);
					}
				}
			}
			else if (streq(command, "max")) {
				printx(2, "Reducing to maximum\n");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						*selection = max_population(*selection, opt_num);
					}
				}
			}
			else if (streq(command, "plot")) {
				plot(selection, selection_type);
			}
			else if (streq(command, "repeat")) {

			}
			else if (streq(command, "quit")) {
				printx(2, "Exiting\n");
				exit(1);
			}
			else {
				printx(2, "Command not recognized\n");
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
			printx(2, "Found command "MAGENTA"%s"RESET" \n", command);
		}
		else if (token[0] == '-') {
			optionc = token[1];
		}
		else if (optionc != '\0') {
			printx(2, "Received option "MAGENTA"-%c"RESET" with value %s \n", optionc, token);
			switch (optionc) {
				case 'n': {
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
			printx(2, "Found unlabeled option\n");
			opt = (char*) strdup(token);
			opt[strcspn(opt, "\n")] = 0;
		}

		printx(2, "getting next token...\n");
		token = strtok(NULL, " ");
	} while (!complete);
}
