.phony: all dir clean test

all:
	make dir
	make exe/bignum

exe/bignum: src/bignum.c
	cc -g $^ -o $@

test: src/test.c
	cc -g $^ -o exe/test

dir:
	mkdir -p exe obj

clean:
	rm -rf exe
	rm -rf obj
