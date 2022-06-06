/* Generated from plot/plot.c0 at 06/05/2022 */
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../helpers/helpers.h"
#include "../state/state.h"
#include "../mainheaders.h"

void plot(void* source, char* type, int level) {
	printx(level, "Plotting data\n");
	char* dpath = "pltdata.txt";
	char* template = "python3.9 plot.py %s";
	int length = snprintf(NULL, 0, template, dpath);
	char* buffer = calloc(length+1, sizeof(char));
	sprintf(buffer, template, dpath);
	char* ex = "python3.9";
	if (streq(type, "state")) {
		FILE* fptr = fopen(dpath, "w");
		write_array(((state*) source) -> data, fptr, level+1);
		fclose(fptr);

		//FILE* p = popen(buffer, "r");
		//int status = pclose(p);
		//
		execlp(ex, ex, "plot/plot.py", dpath, (char*) 0);
	} else if (streq(type, "array")) {
		FILE* fptr = fopen(dpath, "w");
		write_array(*((array*) source), fptr, level+1);
		fclose(fptr);
		execlp(ex, ex, "plot/plot.py", dpath, (char*) 0);
	} else {
		//assert(0, "invalid type");
	}
	free(buffer);
	printx(level, "Done\n");
}
