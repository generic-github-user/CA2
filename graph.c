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
