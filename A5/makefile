.phony: all dir clean test

all: src/decomment.l
	make dir
	make exe/decomment

exe/decomment: obj/decomment.yy.o obj/decomment.o
	cc $^ -o $@

obj/decomment.o: src/decomment.c
	cc -c $^ -o $@ -I src

obj/decomment.yy.o: obj/decomment.yy.c
	cc -c $^ -I src -o $@ 

obj/decomment.yy.c: src/decomment.l
	lex -o $@ -I src $^

dir:
	mkdir -p exe obj

clean:
	rm -rf exe
	rm -rf obj
