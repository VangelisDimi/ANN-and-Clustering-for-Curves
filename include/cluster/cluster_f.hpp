#pragma once
#ifndef cluster_f_h
#define cluster_f_h

using namespace std;
#include <vector>
#include <list>

//Implementation of centroid-based clustering
//K-Means++ Initialization
//DFD Metric
//Update centroids by calculating mean curve

class cluster_Frechet
{
public:
    struct centroid_item
    {
        vector<vector<float>> p;
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
        vector<vector<float>> coordinates;
        vector<centroid_item> curves;
    };
    
    vector<centroid> get_clusters();
    pair<vector<float>,float> get_silhouettes_average();

protected:
    vector<centroid> centroids;
    int K;

    int vectorSize;
    int n;
    int iterations = 0;
    cluster_Frechet(int K,vector<vector<vector<float>>> curves);
    vector<vector<float>> calculateMeanCurve(int i);
    void new_centroids();
    bool convergence(vector<centroid> centroids_old);
};

class cluster_lloyds_Frechet : public cluster_Frechet
{
public:
    cluster_lloyds_Frechet(int K,vector<vector<vector<float>>> curves);
};

#endif