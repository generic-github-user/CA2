#include <stdlib.h>
#include "list.h"
#include "../helpers/helpers.h"

// TODO: use number ranges for more efficient handling of fragmented lists

// Initialize a new list
list* new_llist(void** data, int size, int block_size, void (*pfunc)(void*)) {
	list* L = malloc(sizeof(list));
	*L = (list) {data, NULL, 0, size, block_size, 0, 0, pfunc};
	return L;
}

// Clear a list from memory
void free_llist(list* L, int free_members) {
	// Iterate over list members and free them individually
	if (free_members) {
		for (int i=0; i<L->size; i++) {
			free(L->data[i]);
		}
	}
	// Free pointers to other properties
	free(L->free_cells);
	free(L->data);
	// Free enclosing struct
	free(L);
}

// Insert an element into the provided list
void llist_add(list* L, void* x) {
	// Allocate more space if necessary
	if (L->size % L->block_size == 0) {
		void** m = realloc(L->data, L->size+L->block_size);
		if (m == NULL) {
			printx(0, "Memory allocation failed; exiting\n");
			exit(1);
		}
		L->data = m;

		L->free_cells = realloc(L->free_cells, L->size+L->block_size);
	}

	if (L->ordered) {
		L->data[L->size++] = x;
	} else {
		// Write to the first available cell and update the appropriate counters
		if (L->nfree == 0) { L->free_cells[L->nfree] = L->size; L->nfree++; }
		L->data[L->free_cells[L->nfree]] = x;
		L->nfree --;
	}
	L->compute ++;
	L->size ++;
}

// Remove an item from the list by its index
void llist_remove(list* L, int index) {
	// not technically necessary if we store free indices
	L->data[index] = NULL;
	L->size --;
	L->compute ++;
	L->free_cells[L->nfree++] = index;
}

list* llist_map(list* L, void* (*f)(void*), int output) {
	list* result = NULL;
	if (output) { result = new_llist(NULL, 0, L->block_size, L->pfunc); }
//	else { result = NULL; };
	for (int i=0; i<L->size; i++) {
		if (output) { llist_add(result, f(L->data[i])); }
		else { f(L->data[i]); }
	}
	return result;
}

void llist_print(FILE* target, list* L) {
	llist_map(L, (void*(*)(void*)) L->pfunc, 0);
}
