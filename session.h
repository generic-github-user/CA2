#include "timeinfo.h"

struct session {
	void** selection; // The user's current selection
	int space, compute; // Memory and operations used during this session
	timeinfo time;
};
typedef struct session session;

session new_session();
