/* Generated from ./typing/typing.c0 at 05/26/2022 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// A struct representing a generic type; instances of this struct describe specific types, which objects may be sampled from or compared against
struct type {
	char* name;
	char** attrs;
	struct type* types;
	int n;

	int (*validator)(void*);
	void* (*generator)();
};
typedef struct type type;

char* type_name(type T) {
	char* result = calloc(100, sizeof(char));
//	strcat(result, T.name);
//	strcat(result, " {");
//	snprintf(result, 100, ""
	
	char* c = result;
	c+=sprintf(c, "%s {", T.name);
	for (int i=0; i<T.n; i++) {
		c+=sprintf(c, "%s%s", type_name(T.types[i]), i<T.n-1?", ":"");
	}
	c+=sprintf(c, "}");
	return result;
}
