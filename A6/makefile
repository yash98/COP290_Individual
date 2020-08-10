#Compiler
CXX = gcc

LINK_FLAGS = -lm

#Libraries common include file directory
COMMON_INCLUDES = -Iinclude

# storing all .c and .o filenames before they are compiled
SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, obj/%.o, $(SRC_FILES))
DEBUG_FILES = $(patsubst src/%.c, debug/%.o, $(SRC_FILES))

.PHONY: all
all:
	make dir;
	make allCompile;

.PHONY: allCompile
allCompile: $(OBJ_FILES)
	$(CXX) $(LINK_FLAGS) $^ -o qSim;

.PHONY: debug
debug:
	make dir;
	make debugCompile;

.PHONY: debugCompile
debugCompile: $(DEBUG_FILES)
	$(CXX) $(LINK_FLAGS) -g $^ -o qSimDebug

obj/%.o: src/%.c
	$(CXX) $(COMMON_INCLUDES) -c $^ -o $@

debug/%.o: src/%.c
	$(CXX) $(COMMON_INCLUDES) -g -c $^ -o $@

.PHONY: dir
dir:
	mkdir -p obj bin debug

.PHONY: clean
clean:
	rm -rf obj bin debug qSim qSimDebug

