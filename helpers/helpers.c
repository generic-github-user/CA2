#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../simulation/simulation.h"
#include ".././mainheaders.h"

extern FILE* logfile;

int inrange(int x, int n, int m) {
	return x >= n && x <= m;
}

// Loosely based on https://en.cppreference.com/w/c/variadic
void printx(int level, const char* fmt, ...) {
//	printf("fmt: %s", fmt);
//void printx(int level, char* text) {
	va_list args;
	va_start(args, fmt);

	char* indent = "  ";
	for (int i=0; i<level; i++) {
		printf("%s", indent);
		fprintf(logfile, "%s", indent);
	}
	int signal = 0;
	while (*fmt != '\0') {
//		printf("fmt: %s", fmt);
		if (*fmt == '%') {
			signal = 1;
		}
//		else if (signal && *fmt == 's') {
		else if (strchr("cis", *fmt) != NULL & signal) {
			char* text = va_arg(args, char*);
			if (text == NULL) { text = "NULL"; }
			char ft[3];
			snprintf(ft, 3, "%%%c", *fmt);

			printf(ft, text);
			fprintf(logfile, ft, text);
			signal = 0;
		} else {
			printf("%c", *fmt);
			fprintf(logfile, "%c", *fmt);
		}
		++fmt;
	}
	va_end(args);
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

void assert(int condition, char* message) {
	if (!condition) {
		printf(RED "Warning: " RESET "%s\n", message);
	}
}
