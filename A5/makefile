.phony: all dir clean test

all: src/decomment.l
	make dir
	make obj/decomment.yy.c

obj/decomment.yy.c: src/decomment.l
	lex $^ -o $@

dir:
	mkdir -p exe obj

clean:
	rm -rf exe
	rm -rf obj
