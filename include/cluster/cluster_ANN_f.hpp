#pragma once
#ifndef cluster_ANN_h
#define cluster_ANN_h
#include "cluster.hpp"
#include "lsh.hpp"
#include "cube.hpp"

//Implementation of clustering with reverse assignment

class cluster_ANN : public cluster
{
protected:
    void bruteforce_assignment(vector<tuple<int,int,float>> flagged_indexes);
    float init_search_radius();
    bool terminationCriterion(float search_radius, int* updatedCentroid);

    vector<vector<float>> vectors;

    cluster_ANN(int K,vector<vector<float>> vectors);
};

class cluster_lsh : public cluster_ANN,public LSH //Using LSH reverse assignment (range search)
{
public:
    cluster_lsh(vector<vector<float>> vectors,int K,int k,int L);
    void new_assignment();
    void rangeSearch_Assignment(vector<tuple<int,int,float>> flagged_indexes, float search_radius);
};

class cluster_cube : public cluster_ANN,public Cube //Using Hypercube reverse assignment (range search)
{
public:
    cluster_cube(vector<vector<float>> vectors,int K,int k,int probes,int M);
    void new_assignment();
    void rangeSearch_Assignment(vector<tuple<int,int,float>> flagged_indexes, float search_radius);
};

#endif