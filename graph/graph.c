/* Generated from graph/graph.c0 at 06/03/2022 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

// Initialize a new graph object, which wraps node structsreal
graph* new_graph(node* nodes, int size) {
	graph* G = malloc(sizeof(graph));
	*G = (graph) {nodes, NULL, size, 0, 0};
	return G;
}

int graph_contains(graph* G, node* n) {
	for (int i=0; i<G->size; i++) {
//		if (G->nodes[i] == n) {
		if (memcmp(G->nodes+i, n, sizeof(node)) == 0) {
			return 1;
		}
		G->compute ++;
	}
	return 0;
}

graph* graph_neighborhood(graph* G, node* n, int degree) {
	// we could also recursively or iteratively go through each node's adjacency list
	graph* H = new_graph(n, 1);
	for (int r=0; r<degree; r++) {
		for (int i=0; i<G->e; i++) {
			if (
				graph_contains(H, (G->edges[i][0])) &&
				!graph_contains(H, (G->edges[i][1]))
			) {
//				new_node(H, n->data);
				H->nodes = (node*) realloc(H->nodes, (H->size+1)*sizeof(node));
				(H->nodes)[H->size++] = *(G->edges[i][1]);
			}
			G->compute ++;
			H->compute ++;
		}
	}
	return H;
}

	

// Initialize a node, which stores a (generic) pointer to some data and sets of adjacenct nodes
node new_node(graph* G, void* data) {
	node n = {data};
	// !!!
	n.indegree = 0;
	n.outdegree = 0;
	// Store the new node in the graph, if one is provided
	if (G != NULL) {
		G->nodes = (node*) realloc(G->nodes, (G->size+1)*sizeof(node));
		(G->nodes)[G->size++] = n;
	}
	n.size = 1;
	return n;
}

// Specify a directed edge from node a to node b,
// updating the appropriate information for both nodes
void add_in(node* a, node* b, graph* G) {
	a->in = realloc(a->indegree ? a->in : NULL, a->indegree+1);
	(a->in)[a->indegree] = b;
	a->indegree ++;

	b->out = realloc(b-> outdegree ? b->out : NULL, b->outdegree+1);
	(b->out)[b->outdegree] = a;
	b->outdegree ++;

	if (G != NULL) {
		G->edges = (node*(*)[2]) realloc(G->edges, (size_t) (G->e+1)*2);
		// TODO: can this be cleaned up? using something like (node(*)[2]) {a, b} threw compiler errors
		(G->edges)[G -> e ++][0] = a;
	}
}

// Initialize a linked list based on the graph API
node* new_list(void* value) {
//	printf("Initializing list with value from %p\n", value);
	graph* G = new_graph(NULL, 0);
	node* n = malloc(sizeof(node));
	*n = new_node(G, value);
//	printf("node {indegree: %i, outdegree: %i}\n", n->indegree, n->outdegree);
	return n;
}

// TODO: add direct addressing for hybrid lists?
// Add a node to the end of a linked list
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
		add_in(n, c, NULL);
	}
}

//array ltoarray(node* list) {
//	array a = new_array(1, 
