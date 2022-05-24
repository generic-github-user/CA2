src = $(wildcard */*.c *.c)
obj = $(patsubst %.c,%.o,$(src))
templates = $(wildcard **/*.ct)
ca: $(obj)
	gcc $(obj) -lm -o ca
%.o: %.c
	rm -f $@
	gcc -g -c $< -o $@
commands.c: $(wildcard *.ct)
%.c: %.c0 $(templates)
	python3.9 build.py

# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
