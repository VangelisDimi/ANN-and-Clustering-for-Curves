#pragma once
#ifndef lsh_h
#define lsh_h

#include <ctime>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <unordered_map>

#include "hash_functions.hpp"
#include "exhaustive_search.hpp"
#include "utils.hpp"
#include "hash_table.hpp"

using namespace std;

#include <iostream>

#define L2 2

class LSH
{
private:
	struct hashtable_item_lsh
	{
		vector<float> p;
		unsigned long long int ID;
		int index;
		bool flag = false;
		float radius_found = 0;
	};

	int n;//Number of vectors
	int tableSize;
	int vectorSize;

	float (*distance)(vector<float>, vector<float>);//Distance function
	hash_table<hashtable_item_lsh> *hashtables;//Hashtables
	int L;//Number of hashtables
	int k;//Number of hash functions

	vector<int> r;//r vector used by g
	vector<float> **v;//Random vector used by hash function
	float **t;//Random number used by hash function
	unsigned int w;//Window used by hash function
	unsigned int M = UINT32_MAX - 4;//M used by g

	unsigned int g(vector<float> p,unsigned int j);
	unsigned long long int ID(vector<float> p,unsigned int j);

protected:
	bool clusterMode = false;
	void unmarkAssignedPoints();

public:
	LSH(vector<vector<float>> input_vectors,int k,int L,int metric,float hashtable_size_ratio = 0.25);
	~LSH();//Destructor
	vector<pair<float,unsigned int>> find_N_nearest(vector<float> p,unsigned int N);
	vector<pair<float,unsigned int>> find_R_nearest(vector<float> p,float R);
};

template class hash_table<LSH::hashtable_item_lsh>;

#endif