ca.o: ca.c TinyPngOut.o TinyPngOut.c TinyPngOut.h
	gcc -g ca.c TinyPngOut.o -lm -o ca.o
TinyPngOut.o: TinyPngOut.c TinyPngOut.h
	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
