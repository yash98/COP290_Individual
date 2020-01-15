PYTHON_LINK := $(shell whereis python | sed 's/ /\n/g' | grep include | tail -n 1 | sed 's/\//\n/g' | grep python)
.phony: run all dir clean

run:
	$(MAKE) all ;\
	echo "running c program from directory 1 and 2" ;\
	./exe/myApp ;\
	echo "running java program from directory 3 and 4" ;\
	cd obj && java Caller 3 cd ..

all:
	$(MAKE) dir ;\
	$(MAKE) -C 1 ;\
	$(MAKE) -C 2 ;\
	$(MAKE) -C 3 ;\
	$(MAKE) -C 4 ;\
	$(MAKE) exe/myApp

exe/myApp: obj/myApp.o obj/myPythonInC.o obj/mySqr.o
	$(CC) -l$(PYTHON_LINK) $^ -o $@

obj/myApp.o: myApp.c
	$(CC) -c $^ -o $@

dir:
	mkdir -p exe obj

clean:
	rm -rf exe obj
