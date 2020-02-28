.phony: clean

conversion: src/lex.yy.c src/y.tab.c
	gcc -g $^ -o $@ -lfl

src/lex.yy.c: src/y.tab.c src/conversion.l
	lex -o $@ src/conversion.l

src/y.tab.c: src/conversion.y
	yacc -o $@ -d $^

clean: 
	rm -rf src/lex.yy.c src/y.tab.c src/y.tab.h conversion
