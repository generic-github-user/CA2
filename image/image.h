#ifndef IMAGE_H
#define IMAGE_H

#include "../array/array.h"
#include "../state/state.h"

struct image {
	array* data;
};
typedef struct image image;

image generate_image(state, char*);
void write_image(state, char*);

#endif
