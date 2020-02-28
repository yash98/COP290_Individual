.phony: all dir clean test

debug = -g

all:
	make dir && \
	make exe/decomment

exe/decomment: obj/lex_decomment.yy.o obj/decomment.o
	cc $(debug) $^ -o $@ -lfl

obj/decomment.o: src/decomment.c
	cc $(debug) -c $^ -o $@ -I src

obj/lex_decomment.yy.o: obj/lex_decomment.yy.c
	cc $(debug) -c $^ -I src -o $@ 

obj/lex_decomment.yy.c: src/lex_decomment.l
	lex -o $@ $^ 

dir:
	mkdir -p exe obj

clean:
	rm -rf exe
	rm -rf obj
