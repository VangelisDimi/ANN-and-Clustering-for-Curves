#pragma once
#ifndef exhaustive_search_h
#define exhaustive_search_h

using namespace std;
#include <vector>

vector<pair<float,unsigned int>> exhaustive_search(vector<float> p,vector<vector<float>> vectors,unsigned int N,float (* distance)(vector<float>,vector<float>));
vector<pair<float,unsigned int>> exhaustive_search(vector<vector<float>> p,vector<vector<vector<float>>> curves,unsigned int N,float (* distance)(vector<vector<float>>,vector<vector<float>>));

#endif