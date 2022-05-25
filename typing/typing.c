/* Generated from ./typing/typing.c0 at 05/25/2022, 02:40:18 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
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
