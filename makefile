#makefile
COMMON 			?= ./src/common
INCLUDE_COMMON 	?= ./include/common

CC				= g++ -std=c++14
CFLAGS 			?= -I$(INCLUDE_COMMON) -O2
DEBUGFLAGS 		?= -g -Wextra -Wall -I$(INCLUDE_COMMON) -O2

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


mkdir:
	mkdir -p ./bin
	mkdir -p ./output

cleanall: clean_search
