#include <stdlib.h>
#include "list.h"
#include "../helpers/helpers.h"

// TODO: use number ranges for more efficient handling of fragmented lists


list* new_llist(void** data, int size, int block_size) {
	list* L = malloc(sizeof(list));
	*L = (list) {data, NULL, 0, size, block_size};
	return L;
}

void free_llist(list* L, int free_members) {
	if (free_members) {
		for (int i=0; i<L->size; i++) {
			free(L->data[i]);
		}
	}
	free(L->free_cells);
	free(L->data);
	free(L);
}

void llist_add(list* L, void* x) {
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
		if (L->nfree == 0) { L->free_cells[L->nfree] = L->size; L->nfree++; }
		L->data[L->free_cells[L->nfree]] = x;
		L->nfree --;
	}
	L->size ++;
}

// Remove an item from the list by its index
void llist_remove(list* L, int index) {
	// not technically necessary if we store free indices
	L->data[index] = NULL;
	L->size --;
	L->free_cells[L->nfree++] = index;
}
