#ifndef GRAPH_H
#define GRAPH_H

// A generic node with a pointer to a value and information about adjacent nodes
struct node {
	void* data; // The data stored in the node -- the datatype and any necessary dereferencing will need to be handled by the user
	struct node** in;
	struct node** out;
	unsigned int indegree;
	unsigned int outdegree;
};
typedef struct node node;

// A graph data structure containing a collection of nodes
struct graph {
	node* nodes;
	int size;
};
typedef struct graph graph;

// Function headers
node new_node(graph*, void*);
void add_in(node*, node*);
node* new_list(void*);
void list_add(node*, void*);

graph* new_graph(node*, int);

#endif
