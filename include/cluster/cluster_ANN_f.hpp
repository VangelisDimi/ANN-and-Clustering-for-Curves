#pragma once
#ifndef cluster_ANN_f_h
#define cluster_ANN_f_h
#include "cluster_f.hpp"
#include "lsh_frechet.hpp"

//Implementation of clustering with reverse assignment

class cluster_ANN_Frechet : public cluster_Frechet
{
protected:
    void bruteforce_assignment(vector<tuple<int,int,float>> flagged_indexes);
    float init_search_radius();
    bool terminationCriterion(float search_radius, int* updatedCentroid);

    vector<vector<vector<float>>> curves;
    int curveSize;

    cluster_ANN_Frechet(int K,vector<vector<vector<float>>> curves);
};

class cluster_lsh_Frechet : public cluster_ANN_Frechet,public LSH_Frechet //Using LSH reverse assignment (range search)
{
public:
    cluster_lsh_Frechet(vector<vector<vector<float>>> curves,int K,int k,int L,double delta);
    void new_assignment();
    void rangeSearch_Assignment(vector<tuple<int,int,float>> flagged_indexes, float search_radius);
};

#endif