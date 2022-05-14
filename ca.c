#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

int main() {
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
