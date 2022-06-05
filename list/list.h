#include <stdio.h>

struct list {
	void** data; // the actual container that stores the list's data; a generic pointer
	int* free_cells; // a list (stack) of available indices
	int nfree; // the number of unused spaces reserved for the data
	int size; // the number of elements in the list
	int block_size; // the number of bytes to be (de)allocated when the list changes size
	int ordered; // whether the list elements must be kept in the order they were added in; if 0, the struct acts as a multiset
	int compute; // number of operations used by this list so far

	void(*pfunc)(void*);
};
typedef struct list list;

// A "view" of a list struct that describes an interface for working with some data stored in a list
// without modifying the original data source, similar to NumPy array views
struct listview {
	list* source;
	range* spans;
	struct listview* sublists;
	int size;
};
typedef struct listview listview;

// Function declarations (see list.c for details)
list* new_llist(void**, int, int, void(*)(void*));
void free_llist(list*, int);
void llist_add(list*, void*);
void llist_remove(list*, int);

list* llist_map(list*, void* (*f)(void*), int);
void llist_print(FILE*, list*);
