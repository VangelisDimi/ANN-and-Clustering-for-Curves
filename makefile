#makefile
all: compile_search

COMMON 			?= ./src/common
INCLUDE_COMMON 	?= ./include/common

LIB_FILES 		?= ./bin/lib/frechet.o ./bin/lib/curve.o ./bin/lib/config.o ./bin/lib/simplification.o ./bin/lib/point.o
INCLUDE_LIB 	?= -I./lib/Fred/include

CC				= g++ -std=c++14
CFLAGS 			?= -I$(INCLUDE_COMMON) $(INCLUDE_LIB) -O2
DEBUGFLAGS 		?= -g -Wextra -Wall -I$(INCLUDE_COMMON) $(INCLUDE_LIB)  -O2

INPUT_FILE 		?= ./examples/Datasets/nasd_input.csv
QUERY_FILE 		?= ./examples/Datasets/nasd_query.csv
CONFIG_FILE     ?= ./examples/cluster_example.conf
OUTPUT_FILE 	?= results
METRIC			?= discrete
ALGORITHM		?= Frechet
UPDATE			?= "Mean Frechet"
ASSIGNMENT      ?= Classic

ARGS_SEARCH 	?= -algorithm $(ALGORITHM) -metric $(METRIC) -i $(INPUT_FILE) -q $(QUERY_FILE) -o $(OUTPUT_FILE)_search
ARGS_CLUSTER 	?= -i $(INPUT_FILE) -o $(OUTPUT_FILE)_cluster -c $(CONFIG_FILE) -update $(UPDATE) -assignment $(ASSIGNMENT) -silhouette -complete

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
			
run_search:
	./bin/search $(ARGS_SEARCH) 

#cluster
clean_cluster:
	rm -f ./bin/cluster

compile_cluster: mkdir
	$(CC) ./src/cluster/main_cluster.cpp ./src/cluster/cluster.cpp ./src/cluster/cluster_ANN.cpp ./src/cube/cube.cpp ./src/lsh/lsh.cpp $(COMMON)/utils.cpp $(COMMON)/hash_functions.cpp \
	 ./src/cluster/cluster_f.cpp ./src/cluster/cluster_ANN_f.cpp ./src/search/lsh_frechet.cpp $(LIB_FILES) \
	 -o ./bin/cluster -I./include/cluster -I./include/lsh -I./include/cube -I./include/search $(CFLAGS)

compile_debug_cluster:
	$(CC) ./src/cluster/main_cluster.cpp ./src/cluster/cluster.cpp ./src/cluster/cluster_ANN.cpp ./src/cube/cube.cpp ./src/lsh/lsh.cpp $(COMMON)/utils.cpp $(COMMON)/hash_functions.cpp \
	 ./src/cluster/cluster_f.cpp ./src/cluster/cluster_ANN_f.cpp ./src/search/lsh_frechet.cpp $(LIB_FILES) \
	 -o ./bin/cluster -I./include/cluster -I./include/lsh -I./include/cube -I./include/search $(DEBUGFLAGS)

valgrind_cluster: mkdir
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=./output/valgrind-out-cluster.txt \
			./bin/cluster $(ARGS)
			
run_cluster:
	./bin/cluster $(ARGS_CLUSTER)

#test
install_test_lib:
	sudo apt-get install libcppunit-dev
remove_test_lib:
	sudo apt-get remove libcppunit-dev

compile_test:
	mkdir -p ./bin/test
	$(CC) ./test/test.cpp ./src/common/utils.cpp \
	-o ./bin/test/test $(CFLAGS) -lcppunit
run_test:
	./bin/test/test


#lib
LIB_FLAGS = -march=native -Ofast -static-libgcc -static-libstdc++ -std=c++14 -fpermissive -fPIC -ffast-math -fno-trapping-math -ftree-vectorize
compile_lib:
	mkdir -p ./bin/lib
	g++ -c ./lib/Fred/src/frechet.cpp -o ./bin/lib/frechet.o -I./lib/Fred/include $(LIB_FLAGS)
	g++ -c ./lib/Fred/src/curve.cpp -o ./bin/lib/curve.o -I./lib/Fred/include $(LIB_FLAGS)
	g++ -c ./lib/Fred/src/config.cpp -o ./bin/lib/config.o -I./lib/Fred/include $(LIB_FLAGS)
	g++ -c ./lib/Fred/src/simplification.cpp -o ./bin/lib/simplification.o -I./lib/Fred/include $(LIB_FLAGS)
	g++ -c ./lib/Fred/src/point.cpp -o ./bin/lib/point.o -I./lib/Fred/include $(LIB_FLAGS)

mkdir:
	mkdir -p ./bin
	mkdir -p ./output

cleanall: clean_search
