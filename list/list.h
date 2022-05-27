struct list {
	void** data; // the actual container that store's the list's data; a generic pointer
	int* free_cells; // a list (stack) of available indices
	int nfree; // the number of unused spaces reserved for the data
	int size; // the number of elements in the list
	int block_size; // the number of bytes to be (de)allocated when the list changes size
	int ordered; // whether the list elements must be kept in the order they were added in; if 0, the struct acts as a multiset
	int compute; // number of operations used by this list so far
};
typedef struct list list;

// Function declarations (see list.c for details)
list* new_llist(void**, int, int);
void free_llist(list*, int);
void llist_add(list*, void*);
void llist_remove(list*, int);

