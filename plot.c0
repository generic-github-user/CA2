#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "helpers.h"
#include "state.h"

void plot(void* source, char* type) {
	printf("Plotting data\n");
	char* dpath = "pltdata.txt";
	char* template = "python3.9 plot.py %s";
	int length = snprintf(NULL, 0, template, dpath);
	char* buffer = calloc(length+1, sizeof(char));
	sprintf(buffer, template, dpath);
	char* ex = "python3.9";
	if (streq(type, "state")) {
		FILE* fptr = fopen(dpath, "w");
		write_array(((state*) source) -> data, fptr);
		fclose(fptr);

		//FILE* p = popen(buffer, "r");
		//int status = pclose(p);
		//
		execlp(ex, ex, "plot.py", dpath, (char*) 0);
	}
	free(buffer);
}
