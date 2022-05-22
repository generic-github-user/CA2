#include "graph.h"

//graph new

node new_node(graph* G, void* data) {
	node n = {data};
	return n;
}

void add_in(node* a, node* b) {
	(a->in)[a->indegree] = b;
	a->indegree ++;
	(b->out)[b->outdegree] = a;
	b->indegree ++;
}

node* new_list(void* value) {
//	printf("Initializing list with value from %p\n", value);
	graph G = {};
	node* n = malloc(sizeof(node));
	*n = new_node(&G, value);
//	printf("node {indegree: %i, outdegree: %i}\n", n->indegree, n->outdegree);
	return n;
}

// TODO: add direct addressing for hybrid lists?
void list_add(node* list, void* value) {
//	printf("Adding data at pointer %p to list with head value %p\n", value, list->data);
	if (list -> data == NULL) {
		list -> data = value;
	} else {
		node* c = list;
//		while (c -> outdegree > 0) {
		while (c -> outdegree != 0) {
			c = (c -> out)[0];
		}
		node* n = malloc(sizeof(node));
		*n = new_node(NULL, value);
		add_in(n, c);
	}
}

//array ltoarray(node* list) {
//	array a = new_array(1, 
