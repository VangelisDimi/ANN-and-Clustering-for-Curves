#makefile
all: compile_search

COMMON 			?= ./src/common
INCLUDE_COMMON 	?= ./include/common
INCLUDE_LIB ?= -I./lib/Fred/pybind11/include -I/usr/include/python2.7 -I./lib/Fred/include

CC				= g++ -std=c++14
CFLAGS 			?= -I$(INCLUDE_COMMON) $(INCLUDE_LIB) -O2
DEBUGFLAGS 		?= -g -Wextra -Wall -I$(INCLUDE_COMMON) $(INCLUDE_LIB) -O2

#search
clean_search:
	rm -f ./bin/search

compile_search: mkdir
	$(CC) ./src/search/main_search.cpp ./src/lsh/lsh.cpp ./src/cube/cube.cpp $(COMMON)/hash_functions.cpp $(COMMON)/utils.cpp $(COMMON)/exhaustive_search.cpp -o ./bin/search -I./include/lsh -I./include/cube $(CFLAGS)

compile_debug_search:
	$(CC) ./src/search/main_search.cpp ./src/lsh/lsh.cpp ./src/cube/cube.cpp $(COMMON)/hash_functions.cpp $(COMMON)/utils.cpp $(COMMON)/exhaustive_search.cpp -o ./bin/search -I./include/lsh -I./include/cube $(DEBUGFLAGS)

valgrind_search: mkdir
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=./output/valgrind-out-lsh.txt \
			./bin/search $(ARGS)
			
run_search: compile_search
	./bin/search $(ARGS)


#lib
compile_lib:
	cd ./lib/Fred && make


mkdir:
	mkdir -p ./bin
	mkdir -p ./output

cleanall: clean_search
