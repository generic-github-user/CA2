ca.o: ca.c TinyPngOut.o TinyPngOut.c TinyPngOut.h
	gcc -g ca.c TinyPngOut.o -lm -o ca.o
colors.o: colors.c
	gcc -g colors.c -o colors.o
TinyPngOut.o: TinyPngOut.c TinyPngOut.h
	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
