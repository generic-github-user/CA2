
src = $(wildcard */*.c *.c)
obj = $(patsubst %.c,%.o,$(src))
templates = $(wildcard **/*.ct)
md = $(wildcard *.md)

all: $(md) ca

ca: $(obj)
	gcc $(obj) -lm -o ca
%.o: %.c
	rm -f $@
	gcc -g -c $< -o $@
commands/commands.c: $(wildcard commands/*.ct)
%.c: %.c0# $(templates)
	python3.9 build.py $<


# %.md: %.src.md
#%.md:# %.src.md# FORCE
$(md): .FORCE
	echo $@
	markdown-toc -i $@
#	markdown-toc $< > $@
.FORCE: ;

# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
