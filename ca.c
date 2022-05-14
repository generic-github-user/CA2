#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// Example commands

// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort population desc > write test.txt, save test.ca2
// abbr DEFAULT "limit compute 10e9 time 60"
// abbr activity "accumulate difference time"
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 50 > sort activity desc > write activity.txt, save activity.ca2
// randomstate -wh 50 -t torus, randomrule -n 100 -c totalistic -r 1 > simulate $ -i 100 > write sample.txt
// randomstate -n 100 -wh 10 > simulate cgol -i 100 -r auto --max 500 > sort area desc > write area.txt, save area.ca2
// snowflake > trace cgol > select -n 20 > write backtracking.txt

int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

struct vector {
	int x, y;
};

struct vector vec(int x, int y) {
	return (struct vector) { x, y };
}

struct array {
	int rank;
	int* shape;
	int size;
	int* data;
};


struct array fill_array(struct array a, int value) {
	for (int i=0; i<a.size; i++) {
		a.data[i] = value;
		compute ++;
	}
	return a;
}

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

//struct array array_and(struct array a1, struct array a2) {

// struct array map_array(struct array a
struct state {

};

struct simulation {

};

int main() {
	printf("ca.c loaded successfully");
	srand(time(NULL));

	int grid[30][30];
	int prev[30][30];
	int neighbors = 0;
	int c, d;

	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			grid[x][y] = rand() % 2;
		}
	}
	for (int i=0; i<200; i++) {
		printf("Simulating frame %i \n", i+1);
		for (int x=0; x<30; x++) {
			for (int y=0; y<30; y++) {
				prev[x][y] = grid[x][y];
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
								neighbors += prev[c][d];
							}
						}
					}
				}
				if (neighbors == 3) { grid[x][y] = 1; }
				if (neighbors < 2 || neighbors > 3) { grid[x][y] = 0; }
			}
		}
		for (int x=0; x<30; x++) {
			for (int y=0; y<30; y++) {
				printf(grid[x][y] ? "*" : " ");
			}
			printf("\n");
		}
		fflush(stdout);
		usleep(100000);
	}
}
