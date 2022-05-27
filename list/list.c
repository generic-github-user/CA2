#include <stdlib.h>

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

//void list_add(list* L)
