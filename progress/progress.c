#include <stdlib.h>
#include <string.h>

#include "progress.h"
#include "../helpers/helpers.h"

int usize = strlen("#");
progress new_progress(float total, int size, int level, int unicode) {
	progress P = (progress) { 0, total, size, level, 0, unicode ? "█" : "#", NULL };
	P.text = calloc(size*usize+10, sizeof(char));
	memset(P.text, ' ', size*usize+10);
	for (int i=1; i<size*usize; i++) {
		if (i % usize == 0) { P.text[i] = ' '; }
	}
	P.text[0] = '['; P.text[size*usize+1] = ']'; P.text[size*usize+9] = '\0';
	return P;
}

void prender(progress* P, int pct) {
	printf("\r");
	if (pct) {
		snprintf(P->text + P->size*usize + 3, 8, "%.2f%%", P->current / P->total * 100);
	}
	printx(P->level, "%s", P->text);
	fflush(stdout);
}

progress* pstep(progress* P, float amount) {
	int q = P->size * ((P->current + amount) / P->total);
//	printf("%i\n", q);
	if (q > (int) P->size * (P->current / P->total)) {
		strncpy(P->text + q*usize, P->fill, usize);
	}
	P->current += amount;
	prender(P, 1);
	if (P->current >= P->total) {
		P->complete = 1;
		printx(0, "\n");
		free(P->text);
	}
	return P;
}
