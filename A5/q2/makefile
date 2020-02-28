.phony: clean

conversion: src/lex.yy.c src/y.tab.c
	gcc -g lex.yy.c y.tab.c -o conversion

src/lex.yy.c: src/y.tab.h src/conversion.l
	lex calc.l

src/y.tab.c: src/conversion.y
	yacc -o $@ -d $^

clean: 
	rm -rf src/lex.yy.c src/y.tab.c src/y.tab.h conversion
