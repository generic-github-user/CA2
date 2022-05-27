// A struct representing an operation that has been or will be carried out;
// intended to store information about simulations and other command
// targets that can be used to monitor the program state and detect problems
struct task {
	char* name;
	void* func(void*, ...);
	struct task** subtasks;
	int compute, space;
};
typedef struct task task;
