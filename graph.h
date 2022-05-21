#ifndef GRAPH_H
#define GRAPH_H

struct node {
	void* data;
	struct node** in;
	struct node** out;
	unsigned int indegree;
	unsigned int outdegree;
};
typedef struct node node;

struct graph {
	node** nodes;
	int size;
};
typedef struct graph graph;

#endif
