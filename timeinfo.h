#include <sys/time.h>

struct timeinfo {
	struct timeval created;
	struct timeval modified;
};
typedef struct timeinfo timeinfo;

timeinfo* init_time();
