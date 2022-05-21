src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))
ca: $(obj)
	gcc $(obj) -lm -o ca
%.o: %.c
	gcc -g -c $< -o $@

# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
