#pragma once
#ifndef cube_h
#define cube_h

#include <ctime>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <chrono>
#include "exhaustive_search.hpp"
#include "utils.hpp"
#include "hash_functions.hpp"
#include "hash_table.hpp"

using namespace std;

#define L2 2

class Cube
{
private:
	struct hashtable_item_cube
	{
		vector<float> p;
		int index;
		bool flag = false;
		float radius_found=0;
	};

	float (*distance)(vector<float>, vector<float>);//Distance function
	hash_table<hashtable_item_cube> *hypercube;
	int k;//Number of hash functions
	int vectorSize;

	vector<float> *v;//Random vector used by hash function
	float *t;//Random number used by hash function
	unsigned int w;//Window used by hash function
	map<int, int> *f_table;

	int argM;
	int probes;

	unsigned int f(int i, int h_p);
	unsigned int F(vector<float> p);

protected:
	bool clusterMode = false;
	void unmarkAssignedPoints();

public:

	Cube(vector<vector<float>> input_vectors,int k,int argM, int probes, int metric);
	~Cube();//Destructor
	vector<pair<float,unsigned int>> find_N_nearest(vector<float> p,unsigned int N);
	vector<pair<float,unsigned int>> find_R_nearest(vector<float> p,float R);
};

template class hash_table<Cube::hashtable_item_cube>;

#endif