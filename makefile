SHELL := /bin/bash

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
#$(md): .FORCE
bq = "\`\`\`"
%.md: %.src.md
	echo $@
	cp $< $@

	echo -e "### Statistics\n" >> $@
	cloc * --exclude-dir node_modules --exclude-ext txt --md --hide-rate --quiet >> $@
	echo -e "\n\n" >> $@

	echo -e "### File tree\n${bq}" >> $@
	tree -aI "node_modules|misc*|.git|cloc*" | cat - <(echo -ne "\n${bq}") >> $@

	markdown-toc -i $@
#	markdown-toc $< > $@
.FORCE: ;

# TinyPngOut.o: TinyPngOut.c TinyPngOut.h
# 	gcc -g -c TinyPngOut.c TinyPngOut.h -o TinyPngOut.o
