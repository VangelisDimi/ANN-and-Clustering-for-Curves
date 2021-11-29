#include <iostream>
#include "exhaustive_search.hpp"
#include "utils.hpp"
#include <map>

vector<pair<float,unsigned int>> exhaustive_search(vector<float> p,vector<vector<float>> vectors,unsigned int N,float (* distance)(vector<float>,vector<float>))
{    
    //Find N nearest neighbors of vector p size n from vector of vectors
    //Return indices
    multimap<float,int> distances;
    for(unsigned int i=0;i<vectors.size();i++)
    {
        distances.insert({distance(p,vectors[i]),i});
    }
    
    vector<pair<float,unsigned int>> nBest;
    unsigned int i=0;
    auto it=distances.begin();
    for(;it != distances.end() && i<N ;++it,i++)
    {
        nBest.push_back({it->first,it->second});
    }
    return nBest;
}