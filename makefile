src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))
ca: $(obj)
	gcc $(obj) -lm -o ca
%.o: %.c
	rm $@
	gcc -g -c $< -o $@
%.c: %.c0
	python3.9 build.py

# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
