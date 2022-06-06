#include <stddef.h>

#include "session.h"
#include "timeinfo.h"

// Create a new session object
session new_session() {
	return (session) {NULL, 0, 0, *init_time()};
}
