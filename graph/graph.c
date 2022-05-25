/* Generated from ./graph/graph.c0 at 05/24/2022, 22:40:57 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

//graph new

node new_node(graph* G, void* data) {
	node n = {data};
	// !!!
	n.indegree = 0;
	n.outdegree = 0;
	return n;
}

void add_in(node* a, node* b) {
	a->in = realloc(a->indegree ? a->in : NULL, a->indegree+1);
	(a->in)[a->indegree] = b;
	a->indegree ++;
	b->out = realloc(b-> outdegree ? b->out : NULL, b->outdegree+1);
	(b->out)[b->outdegree] = a;
	b->outdegree ++;
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
