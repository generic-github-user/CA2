#include <sys/time.h>
#include <stdlib.h>

struct timeinfo {
	struct timeval created;
	struct timeval modified;
};
typedef struct timeinfo timeinfo;

timeinfo* init_time() {
	timeinfo* t = (timeinfo*) malloc(sizeof(timeinfo));
	gettimeofday(&t->created, NULL);
	return t;
}
