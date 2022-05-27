struct session {
	void** selection;
	int space, compute;
};
typedef struct session session;

session new_session();
