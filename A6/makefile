#Compiler
CXX = gcc

#Libraries common include file directory
COMMON_INCLUDES = -Iinclude

# storing all .c and .o filenames before they are compiled
SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, obj/%.o, $(SRC_FILES))

.PHONY: all
all:
	make dir;
	make allCompile;

.PHONY: allCompile
allCompile: $(OBJ_FILES)
	$(CXX) $^ -o qSim;

.PHONY: debug
debug:
	make dir;
	make debugCompile;

.PHONY: debugCompile
debugCompile: $(OBJ_FILES)
	$(CXX) -g $^ -o qSimDebug

obj/%.o: src/%.c
	$(CXX) $(COMMON_INCLUDES) -c $^ -o $@

.PHONY: dir
dir:
	mkdir -p obj bin

.PHONY: clean
clean:
	rm -rf obj bin qSim

