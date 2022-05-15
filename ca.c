#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_REDUCE(name,type,op,init) type name(struct array a) { \
	type output = init;\
	for (int i=0; i<a.size; i++) {\
		output = output op a.data[i];\
	}\
	return output;\
}

// Example commands

// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort population desc > write test.txt, save test.ca2
// abbr DEFAULT "limit compute 10e9 time 60"
// abbr activity "accumulate difference time"
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort activity desc > write activity.txt, save activity.ca2
// randomstate -wh 50 -t torus, randomrule -n 100 -c totalistic -r 1 > simulate $ -i 100 > write sample.txt
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 500 > sort area desc > write area.txt, save area.ca2
// snowflake > trace cgol > select -n 20 > write backtracking.txt

int compute;

// Clip an integer to a range (modify in-place)
int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

// A vector or coordinate
struct vector {
	int x, y;
};

struct vector vec(int x, int y) {
	return (struct vector) { x, y };
}

// A general array struct for multidimensional arrays
struct array {
	int rank;
	int* shape;
	int size;
	int* data;
};

// Fill an array with a value
struct array fill_array(struct array a, int value) {
	for (int i=0; i<a.size; i++) {
		a.data[i] = value;
		compute ++;
	}
	return a;
}

// Initialize an array struct
struct array new_array(int rank, int* shape) {
	// int rank = sizeof(shape);
	// int* size = malloc(sizeof(int));
	int size = 1;
	for(int i=0; i<rank; i++) {
		size *= shape[i];
		compute ++;
	}
	printf("Initalizing array with size %i \n", size);
	//int data[size] = {0};
	//int data[size];
	int* data = calloc(size, sizeof(int));
	struct array a = { rank, shape, size, data };
	fill_array(a, 0);
	return a;
};

int get_coord(struct array a, struct vector z) {
	return z.x * a.shape[0] + z.y;
}

int array_get(struct array a, struct vector z) {
	return a.data[get_coord(a, z)];
}

void array_set(struct array a, struct vector z, int value) {
	a.data[get_coord(a, z)] = value;
}

// struct array array_from(int rank, int* shape, void* values) {


//struct array array_and(struct array a1, struct array a2) {

// void map_array(struct array a, )

void* reduce_array(struct array a, void* (F)(void*, void*), void* init) {
	void* output = init;
	for (int i=0; i<a.size; i++) {
		output = F(output, &a.data[i]);
	}
	return output;
}

// void* sum(int a, int b) { return (void*) a + b; }
// int array_sum(struct array a) { return (int) reduce_array(a, sum, 0); }
ARRAY_REDUCE(array_sum, int, +, 0)

// A static "frame" of a simulation to which the rules of a cellular automata may be repeatedly applied in a simulation
struct state {

};

// A series of frames along with a simulation rule that describes the transition from one state to another (possibly contains additional information)
struct simulation {

};

int main() {
	printf("ca.c loaded successfully");
	// Set random seed
	srand(time(NULL));

	int w = 30;
	int h = 30;
	// int grid[w][h];
	// int prev[w][h];
	int shape[2] = {30, 30};
	struct array grid = new_array(2, shape);
	struct array prev = new_array(2, shape);
	int neighbors = 0;
	compute = 0;
	int c, d;

	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array_set(grid, vec(x, y), rand() % 2);
			compute ++;
		}
	}
	for (int i=0; i<200; i++) {
		printf("Simulating frame %i \n", i+1);
		printf("Total compute: %i \n", compute);
		printf("Population: %i \n", array_sum(grid));
		printf("\n");

		for (int x=0; x<30; x++) {
			for (int y=0; y<30; y++) {
				struct vector v = vec(x, y);
				array_set(prev, v, array_get(grid, v));
				compute ++;
			}
		}
		for (int x=0; x<30; x++) {
			for (int y=0; y<30; y++) {
				neighbors = 0;
				for (int a=-1; a<=1; a++) {
					for (int b=-1; b<=1; b++) {
						if (a!=0 || b!=0) {
							c = x+a;
							d = y+b;
							//bound(&c, 0, 29);
							//bound(&d, 0, 29);
							if (c >= 0 && c <= 29 && d >= 0 && d <= 29) {
								neighbors += array_get(prev, vec(c, d));
							}
						}
						compute ++;
					}
				}
				if (neighbors == 3) { array_set(grid, vec(x, y), 1); }
				if (neighbors < 2 || neighbors > 3) { array_set(grid, vec(x, y), 0); }
			}
		}
		for (int x=0; x<30; x++) {
			for (int y=0; y<30; y++) {
				printf(array_get(grid, vec(x, y)) ? "*" : " ");
			}
			printf("\n");
		}
		printf("\n");
		fflush(stdout);
		usleep(100000);
	}
}
