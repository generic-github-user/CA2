/* Generated from ./image/image.c0 at 05/23/2022, 08:35:13 */ 
/* This is a content file generated from a source (.c0) file; you should edit that file instead */ 
#include <stdlib.h>

#include "TinyPngOut.h"
#include "array.h"
#include "helpers.h"
#include "image.h"
#include "state.h"

image generate_image(state s, char* color) {
//	int shape[3] = {100, 100, 3};
	int* shape = calloc(3, sizeof(int));
	shape = (int[]) {100, 100, 3};
	array* A = malloc(sizeof(array));
	*A = new_array(3, shape);
	image result = { A };
	fill_array(*result.data, 255);
	int w = 3;
	int h = 3;
	int v[1] = {3};
	for (int x=0; x<30; x++) {
		for (int y=0; y<30; y++) {
			array cell_color = new_array(1, v);
			if (streq(color, "age")) {

			}
			if (array_get(s.data, vec(x, y, 0)) != 0) {
//				fill_slice(&s.data, vec(x*w, y*h), vec(x*w+w, y*h+h), 0);
				fill_slice(result.data, vec(x*w, y*h, 0), vec(x*w+w, y*h+h, 3), 0);
			}
		}
	}
	return result;
}

void write_image(state s, char* color) {
	// Based on code from https://www.nayuki.io/res/tiny-png-output/MandelbrotPng.c
	printx(3, "Opening image file\n");
	FILE* target = fopen("test.png", "wb");
	if (target == NULL) {
		perror("Error: fopen");
		exit(1);
	}

	printx(3, "Generating image array");
	image image_data = generate_image(s, color);
	// Why does this modify the array's shape (or cause a segfault)?
	// array_summary(image_data.data);

	struct TinyPngOut pngout;
	static const int width = 100;
	static const int height = 100;
	enum TinyPngOut_Status status = TinyPngOut_init(&pngout, (uint32_t)width, (uint32_t)height, target);
	if (status != TINYPNGOUT_OK) {
		printx(3, "Error in TinyPngOut; exiting");
		exit(1);
	}
	printx(3, "Writing pixels to image\n");

	uint8_t *line = calloc((size_t)width * 3, sizeof(uint8_t));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
//			int pixel = image_data.data -> data[(x*100+y)*3];
			for (int z = 0; z < 3; z++) {
				int pixel = array_get(*image_data.data, vec(x, y, z));
				line[x*3+z] = pixel;
			}
		}
		status = TinyPngOut_write(&pngout, line, (size_t)width);
		if (status != TINYPNGOUT_OK) {
			printx(3, "Encountered error while writing image; exiting");
			exit(1);
		}
	}
	printf("%s", " -- Done");

	free(line);
	free(image_data.data -> data);
}

