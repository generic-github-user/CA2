struct progress {
	float current;
	float total;
	int size;
	int level;
	int complete;
};
typedef struct progress progress;

progress new_progress(float, int, int);
progress pstep(progress, float);
