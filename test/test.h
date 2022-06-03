struct test {
	int passed; // a truth value indicating whether the test completed successfully
	struct test* subtests; // a set of "member" tests that this test depends on
	int nsub; // number of subtests
};
typedef struct test test;
