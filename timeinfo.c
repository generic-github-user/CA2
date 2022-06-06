#include <sys/time.h>
#include <stdlib.h>

#include "timeinfo.h"

timeinfo* init_time() {
	timeinfo* t = (timeinfo*) malloc(sizeof(timeinfo));
	gettimeofday(&t->created, NULL);
	gettimeofday(&t->modified, NULL);
	return t;
}
