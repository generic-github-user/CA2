obj = $(wildcard *.o)
ca: $(obj)
	gcc $(obj) -lm -o ca
%.o: %.c
	gcc -c $< -o $@
# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
