.phony: all dir clean

all:
	make dir
	make exe/bignum

exe/bignum: src/bignum.c
	cc -g $^ -o $@

dir:
	mkdir -p exe obj

clean:
	rm -rf exe
	rm -rf obj
