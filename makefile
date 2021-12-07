#makefile
all: compile_search

COMMON 			?= ./src/common
INCLUDE_COMMON 	?= ./include/common

LIB_FILES ?= ./bin/lib/frechet.o ./bin/lib/curve.o ./bin/lib/config.o ./bin/lib/simplification.o ./bin/lib/point.o
INCLUDE_LIB ?= -I./lib/Fred/include

CC				= g++ -std=c++14
CFLAGS 			?= -I$(INCLUDE_COMMON) $(INCLUDE_LIB) -O2
DEBUGFLAGS 		?= -g -Wextra -Wall -I$(INCLUDE_COMMON) $(INCLUDE_LIB)  -O2

#search
clean_search:
	rm -f ./bin/search

compile_search: mkdir
	$(CC) ./src/search/main_search.cpp ./src/lsh/lsh.cpp ./src/cube/cube.cpp ./src/search/lsh_frechet.cpp $(COMMON)/hash_functions.cpp $(COMMON)/utils.cpp $(COMMON)/exhaustive_search.cpp $(LIB_FILES) -o ./bin/search -I./include/lsh -I./include/cube -I./include/search $(CFLAGS)

compile_debug_search:
	$(CC) ./src/search/main_search.cpp ./src/lsh/lsh.cpp ./src/cube/cube.cpp ./src/search/lsh_frechet.cpp $(COMMON)/hash_functions.cpp $(COMMON)/utils.cpp $(COMMON)/exhaustive_search.cpp $(LIB_FILES) -o ./bin/search -I./include/lsh -I./include/cube -I./include/search $(DEBUGFLAGS)

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
	mkdir -p ./bin/lib
	g++ -c ./lib/Fred/src/frechet.cpp -o ./bin/lib/frechet.o -I./lib/Fred/include
	g++ -c ./lib/Fred/src/curve.cpp -o ./bin/lib/curve.o -I./lib/Fred/include
	g++ -c ./lib/Fred/src/config.cpp -o ./bin/lib/config.o -I./lib/Fred/include
	g++ -c ./lib/Fred/src/simplification.cpp -o ./bin/lib/simplification.o -I./lib/Fred/include
	g++ -c ./lib/Fred/src/point.cpp -o ./bin/lib/point.o -I./lib/Fred/include
	   

mkdir:
	mkdir -p ./bin
	mkdir -p ./output

cleanall: clean_search
