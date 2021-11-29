#include "hash_functions.hpp"
#include <numeric>
#include <cmath>
#include <iostream>

int hash_L2(unsigned int i,vector<float> p,vector<float> *v,float *t,int w) {
	float innerProduct = inner_product(p.begin(), p.end(), v[i].begin(), 0);
	return floor((innerProduct+t[i])/w);
}