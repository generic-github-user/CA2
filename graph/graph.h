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

// typedef (node*(*)[2]) edge;

// A graph data structure containing a collection of nodes
struct graph {
	node* nodes;
	node* (* edges)[2];
	int size;
	int e;
};
typedef struct graph graph;


// Function headers
node new_node(graph*, void*);
void add_in(node*, node*, graph*);
node* new_list(void*);
void list_add(node*, void*);

graph* new_graph(node*, int);
graph* graph_neighborhood(graph*, node*, int);
int graph_contains(graph*, node*);

#endif
