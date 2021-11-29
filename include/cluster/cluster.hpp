#pragma once
#ifndef cluster_h
#define cluster_h

using namespace std;
#include <vector>
#include <list>

//Implementation of centroid-based clustering
//K-Means++ Initialization
//L2 Metric
//Update centroids by calculating mean vector

class cluster
{
public:
    struct centroid_item
    {
        vector<float> p;
		int index;

        bool operator < (const centroid_item& str) const
        {
            return (index < str.index);
        }
        bool operator == (const centroid_item& str) const
        {
            return (index == str.index);
        }
    };

    struct centroid
    {
        vector<float> coordinates;
        vector<centroid_item> vectors;
    };
    
    vector<centroid> get_clusters();
    pair<vector<float>,float> get_silhouettes_average();

protected:
    vector<centroid> centroids;
    int K;

    int vectorSize;
    int n;
    
    cluster(int K,vector<vector<float>> vectors);

    void new_centroids();
    bool convergence(vector<centroid> centroids_old);
};

class cluster_lloyds : public cluster
{
public:
    cluster_lloyds(int K,vector<vector<float>> vectors);
};

#endif