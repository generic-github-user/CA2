struct progress {
	float current;
	float total;
	int size;
	int level;
	int complete;
	char* fill;
	char* text;
};
typedef struct progress progress;

progress new_progress(float, int, int, int);
void prender(progress*, int);
progress* pstep(progress*, float);
