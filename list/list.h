struct list {
	void** data;
	int* free_cells;
	int nfree;
	int size;
	int block_size;
	int ordered;
};
typedef struct list list;

list* new_llist(void**, int, int);
void free_llist(list*, int);
void llist_add(list*, void*);
void llist_remove(list*, int);

