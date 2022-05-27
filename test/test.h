struct test {
	int passed;
	struct test* subtests;
	int nsub;
};
typedef struct test test;
