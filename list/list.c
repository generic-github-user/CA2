#include <stdlib.h>
#include "../helpers/helpers.h"

struct list {
	void** data;
	int size;
	int block_size;
};
typedef struct list list;

list* new_llist(void** data, int size, int block_size) {
	list* L = malloc(sizeof(list));
	*L = (list) {data, size, block_size};
	return L;
}

void llist_add(list* L, void* x) {
	if (L->size % L->block_size == 0) {
		void** m = realloc(L->data, L->size+L->block_size);
		if (m == NULL) {
			printx(0, "Memory allocation failed; exiting\n");
			exit(1);
		}
		L->data = m;
	}
	L->data[L->size++] = x;
}
