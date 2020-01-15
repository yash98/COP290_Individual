PYTHON_INCLUDE := $(shell whereis python | sed 's/ /\n/g' | grep include | tail -n 1)
.phony: all

all:
	$(MAKE) ../obj/myPythonInC.o

../obj/myPythonInC.o: myPythonInC.c
	$(CC) -I$(PYTHON_INCLUDE) -c $^ -o $@
