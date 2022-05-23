#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../state/state.h"
#include "../helpers/helpers.h"
#include "../image/image.h"
#include "../plot/plot.h"

// Check if a string is a valid command name
int iscommand(char* text) {
	char* commands[] = {
		"randomstate", "enumerate",
		"simulate",
		"collapse", "min", "max",
		"write", "print", "render", "table",
		"quit",
		"plot"
	};
	for (int i=0; i<12; i++) {
		if (strcmp(text, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

// Execute a command string, writing to stdout and the provided log file
void process_command(char* cmd, FILE* log) {
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
					*selection = (state*) random_state(opt_shape);
					selection_type = "state";
			}
				else {
					selection = calloc(opt_num, sizeof(state));
					for (int j=0; j<opt_num; j++) {
						*(selection+j) = random_state(opt_shape);
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
				selection = calloc(opt_num, sizeof(state));
				//}
				selection[0] = new_state(new_array(2, opt_shape), NULL);
				// TODO: update stats?
				state* s;
				while (i < opt_num) {
					printx(3, "");
					printf("Generating state %i\n", i);

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
				printx(2, "");
				printf("Writing to output file [%s] \n", opt);
				FILE* outfile = fopen(opt, "w");
				if (strcmp(selection_type, "state") == 0) {
					write_state(*((state*) *selection), outfile);
					//free(state_selection);
				}
				else if (strcmp(selection_type, "state_set") == 0) {
					printx(3, "");
					printf("Writing %i states \n", opt_num);
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
				printx(2, "Executing simulation");
				if (streq(selection_type, "state")) {
					*selection = new_simulation(*((state*) selection), opt_iterations);
					selection_type = "simulation";
					simulate(*selection, opt_iterations, opt_print, 2, opt_unicode, opt_color[0], !opt_print);
				}
				else if (streq(selection_type, "state_set")) {
					selection = calloc(opt_num, sizeof(simulation));
					for (int j=0; j<opt_num; j++) {
						selection[j] = new_simulation(*((state*) selection[j]), opt_iterations);
						simulate(selection[j], opt_iterations, opt_print, 2, opt_unicode, opt_color[0], !opt_print);
					}
					selection_type = "simulation_set";
				}
			}
			else if (streq(command, "collapse")) {
				printx(2, "Collapsing simulation(s)");
				if (streq(selection_type, "simulation_set")) {
					selection = calloc(opt_num, sizeof(state));
					for (int j=0; j<opt_num; j++) {
						//simulation sim = simset_selection[j];
						simulation* sim = selection[j];
						printx(2, "");
						printf("Getting state at index %i\n", sim -> time);
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
					printx(2, "Command not supported for this data type");
				}
			}
			else if (streq(command, "min")) {
				printx(2, "Reducing to minimum");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						*selection = min_population(*selection, opt_num);
					}
				}
			}
			else if (streq(command, "max")) {
				printx(2, "Reducing to maximum");
				if (streq(selection_type, "state_set")) {
					selection_type = "state";
					if (streq(opt, "population")) {
						// TODO
						*selection = max_population(*selection, opt_num);
					}
				}
			}
			else if (streq(command, "plot")) {
				plot(&selection, "state");
			}
			else if (streq(command, "repeat")) {

			}
			else if (streq(command, "quit")) {
				printx(2, "Exiting");
				exit(1);
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
