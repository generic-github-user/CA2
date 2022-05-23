/* Generated from ./helpers/helpers.c0 at 05/23/2022, 08:35:13 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../simulation/simulation.h"

extern FILE* logfile;

int inrange(int x, int n, int m) {
	return x >= n && x <= m;
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

int streq(char* a, char* b) {
	return strcmp(a, b) == 0;
}
