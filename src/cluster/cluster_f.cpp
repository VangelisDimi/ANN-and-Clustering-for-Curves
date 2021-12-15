#include <map>
#include <set>
#include <numeric>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>
#include "cluster_f.hpp"
#include "utils.hpp"
#include<iostream>

//Cluster parent class
cluster_Frechet::cluster_Frechet(int K,vector<vector<vector<float>>> curves)
{
    cluster_Frechet::K=K;
    cluster_Frechet::vectorSize=(!curves.empty()) ? curves[0].size() : 0;
    cluster_Frechet::n=curves.size();

    //K-Means++ initialization
    vector<int> non_centroids(n);
    iota(non_centroids.begin(), non_centroids.end(), 0);

    vector<float> D(n);
    fill(D.begin(), D.end(), numeric_limits<float>::max());

    centroid c1;
    int index=uniform_distribution_rng(0,n-1);
    c1.coordinates=curves[index];
    D[index]=0;
    non_centroids.erase(remove(non_centroids.begin(), non_centroids.end(), index), non_centroids.end());
    centroids.push_back(c1);
    for(int t=1;t<K;t++)
    {
        centroid c;
        for (int i : non_centroids)
        {
            float distance=getDiscreteFrechetDistance(curves[i],centroids.back().coordinates);
            if(distance<D[i]) 
            {
                D[i]=distance;
            }
        }
        float max_value=*max_element( D.begin(), D.end() );
        
        list<pair<float,int>> P;
        float range;
        for (int r : non_centroids)
        {
            float P_r = 0;
            for(int i:non_centroids)
            {
                if(i>r) break;
                P_r+=pow(D[r]/max_value,2);
            }
            P.push_back({P_r,r});
            range=P_r;
        }
        float x=uniform_distribution_rng_float(0,range);

        for (auto it = P.begin(); it != P.end(); ++it)
        {
            auto it_next=next(it,1);
            if(x>it->first && x<=it_next->first)
            {
                centroid c;
                c.coordinates=curves[it_next->second];
                D[it_next->second]=0;
                non_centroids.erase(remove(non_centroids.begin(), non_centroids.end(), it_next->second), non_centroids.end());
                centroids.push_back(c);
                break;
            }
        }
    }
}

void cluster_Frechet::vector<vector<float>> calculateMeanCurve(int i)
{
    tree = Tree(centroids[i].curves);
    return tree.postOrderTraversal();
}

void cluster_Frechet::new_centroids()
{
    //Create new centroids by calculating mean curve
    vector<vector<float>> new_centroids;
    for (int i=0;i<K;i++)
    {
        centroids[i].coordinates = calculateMeanCurve(i);
        centroids[i].curves.clear();
    }
};


vector<cluster_Frechet::centroid> cluster_Frechet::get_clusters()
{
    return centroids;
}

pair<vector<float>,float> cluster_Frechet::get_silhouettes_average()
{
    //Return average silhouettes for every cluster
    //Plus average for whole dataset
    vector<float> averages;
    float total_average=0;
    int total_number=0;

    for (int i = 0; i < K; i++)
    {
        float averages_si=0;
        for (int v = 0; v < centroids[i].curves.size(); v++)
        {
            float a_vector;
            float b_vector;
            for (int a = 0; a < centroids[i].curves.size(); a++)
            {
                if(a==v) continue;
                a_vector+=getDiscreteFrechetDistance(centroids[i].curves[v].p,centroids[i].curves[a].p);
            }
            a_vector/=centroids[i].curves.size()-1;

            float minimum=numeric_limits<float>::max();
            int minimum_index;
            for (int c=0;c<centroids.size();c++)
            {
                if (c==i) continue;
                
                float distance=getDiscreteFrechetDistance(centroids[i].curves[v].p,centroids[c].coordinates);
                if(distance<minimum)
                {
                    minimum=distance;
                    minimum_index=c;
                }
            }
            for (int b = 0; b < centroids[minimum_index].curves.size(); b++)
            {
                b_vector+=getDiscreteFrechetDistance(centroids[i].curves[v].p,centroids[minimum_index].curves[b].p);
            }
            b_vector/=centroids[minimum_index].curves.size();

            float si=(b_vector-a_vector)/max(a_vector,b_vector);
            averages_si+=si;
            total_average+=si;
            total_number++;
        }
        averages_si/=centroids[i].curves.size();
        averages.push_back(averages_si);
    }

    total_average/=total_number;
    return {averages,total_average};
}

bool cluster_Frechet::convergence(vector<centroid> centroids_old)
{
    //Check if centroids converge by checking if they have the same
    //elements as previous assignment
    for (int i = 0; i < K; i++)
    {   
        if(centroids[i].curves.size() != centroids_old[i].curves.size())
                return false;
        sort(centroids_old[i].curves.begin(),centroids_old[i].curves.end());
        sort(centroids[i].curves.begin(), centroids[i].curves.end());

        if(centroids[i].curves!=centroids_old[i].curves) return false;
    }
    return true;
}

//Cluster Lloyd's
cluster_lloyds_Frechet::cluster_lloyds_Frechet(int K,vector<vector<vector<float>>> curves) : cluster_Frechet(K,curves)
{
    //First assignment
    for(int i=0;i<n;i++)
    {
        centroid_item ci={p:curves[i],index:i};
        float minimum=numeric_limits<float>::max();
        int minimum_index;
        for (int v=0;v<K;v++)
        {
            float distance=getDiscreteFrechetDistance(curves[i],centroids[v].coordinates);
            if(distance<minimum)
            {
                minimum=distance;
                minimum_index= v;
            }
        }
        centroids[minimum_index].curves.push_back(ci);
    }
    //Assignment/Update
    while(true)
    {
        vector<centroid> centroids_old=centroids;
        new_centroids();

        for(int i=0;i<n;i++)
        {
            centroid_item ci={p:curves[i],index:i};
            float minimum=numeric_limits<float>::max();
            int minimum_index;
            for (int v=0;v<K;v++)
            {
                float distance=getDiscreteFrechetDistance(curves[i],centroids[v].coordinates);
                if(distance<minimum)
                {
                    minimum=distance;
                    minimum_index= v;
                }
            }
            centroids[minimum_index].curves.push_back(ci);
        }

        if(convergence(centroids_old)==true)
            break;
    }
}