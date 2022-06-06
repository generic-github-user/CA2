struct progress {
	float current;
	float total;
	int size;
	int level;
	int complete;
	char* fill;
};
typedef struct progress progress;

progress new_progress(float, int, int, int);
progress pstep(progress, float);
