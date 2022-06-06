#include "progress.h"
#include "../helpers/helpers.h"

progress new_progress(float total, int size, int level, int unicode) {
	progress P = (progress) { 0, total, size, level, 0, unicode ? "█" : "#" };
	printx(level, "[");
	return P;
}

progress pstep(progress P, float amount) {
	int q = P.size * ((P.current + amount) / P.total);
	if (q > (int) P.size * (P.current / P.total)) {
		printx(0, P.fill);
	}
	P.current += amount;
	if (P.current >= P.total) {
		P.complete = 1;
		printx(0, "]\n");
	}
	return P;
}
