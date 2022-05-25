#ifndef GRAPH_H
#define GRAPH_H

struct node {
	void* data;
	struct node** in;
	struct node** out;
	unsigned int indegree;
	unsigned int outdegree;
	int size;
};
typedef struct node node;

struct graph {
	node** nodes;
	int size;
};
typedef struct graph graph;

node new_node(graph*, void*);
void add_in(node*, node*);
node* new_list(void*);
void list_add(node*, void*);

#endif
