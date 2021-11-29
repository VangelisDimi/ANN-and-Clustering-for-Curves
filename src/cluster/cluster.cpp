#include <map>
#include <set>
#include <numeric>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>
#include "cluster.hpp"
#include "utils.hpp"

//Cluster parent class
cluster::cluster(int K,vector<vector<float>> vectors)
{
    cluster::K=K;
    cluster::vectorSize=(!vectors.empty()) ? vectors[0].size() : 0;
    cluster::n=vectors.size();

    //K-Means++ initialization
    vector<int> non_centroids(n);
    iota(non_centroids.begin(), non_centroids.end(), 0);

    vector<float> D(n);
    fill(D.begin(), D.end(), numeric_limits<float>::max());

    centroid c1;
    int index=uniform_distribution_rng(0,n-1);
    c1.coordinates=vectors[index];
    D[index]=0;
    non_centroids.erase(remove(non_centroids.begin(), non_centroids.end(), index), non_centroids.end());
    centroids.push_back(c1);
    for(int t=1;t<K;t++)
    {
        centroid c;
        for (int i : non_centroids)
        {
            float distance=eucledian_distance(vectors[i],centroids.back().coordinates);
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
                c.coordinates=vectors[it_next->second];
                D[it_next->second]=0;
                non_centroids.erase(remove(non_centroids.begin(), non_centroids.end(), it_next->second), non_centroids.end());
                centroids.push_back(c);
                break;
            }
        }
    }
}

void cluster::new_centroids()
{
    //Create new centroids by calculating mean vector
    vector<vector<float>> new_centroids;
    for (auto it = centroids.begin(); it != centroids.end(); ++it)
    {
        vector<float> new_centroid(vectorSize);
        iota(new_centroid.begin(), new_centroid.end(), 0);
        for(auto it2 = it->vectors.begin() ; it2 != it->vectors.end(); ++it2)
        {
            for (int i = 0; i < vectorSize; i++)
            {
                new_centroid[i]+=it2->p[i];
            }
        }
        for (int y = 0; y < vectorSize; y++)
        {
            new_centroid[y] /= it->vectors.size();
        }
        new_centroids.push_back(new_centroid);
    }
    for (int i=0;i<K;i++)
    {
        centroids[i].coordinates=new_centroids[i];
        centroids[i].vectors.clear();
    }
};

vector<cluster::centroid> cluster::get_clusters()
{
    return centroids;
}

pair<vector<float>,float> cluster::get_silhouettes_average()
{
    //Return average silhouettes for every cluster
    //Plus average for whole dataset
    vector<float> averages;
    float total_average=0;
    int total_number=0;

    for (int i = 0; i < K; i++)
    {
        float averages_si=0;
        for (int v = 0; v < centroids[i].vectors.size(); v++)
        {
            float a_vector;
            float b_vector;
            for (int a = 0; a < centroids[i].vectors.size(); a++)
            {
                if(a==v) continue;
                a_vector+=eucledian_distance(centroids[i].vectors[v].p,centroids[i].vectors[a].p);
            }
            a_vector/=centroids[i].vectors.size()-1;

            float minimum=numeric_limits<float>::max();
            int minimum_index;
            for (int c=0;c<centroids.size();c++)
            {
                if (c==i) continue;
                
                float distance=eucledian_distance(centroids[i].vectors[v].p,centroids[c].coordinates);
                if(distance<minimum)
                {
                    minimum=distance;
                    minimum_index=c;
                }
            }
            for (int b = 0; b < centroids[minimum_index].vectors.size(); b++)
            {
                b_vector+=eucledian_distance(centroids[i].vectors[v].p,centroids[minimum_index].vectors[b].p);
            }
            b_vector/=centroids[minimum_index].vectors.size();

            float si=(b_vector-a_vector)/max(a_vector,b_vector);
            averages_si+=si;
            total_average+=si;
            total_number++;
        }
        averages_si/=centroids[i].vectors.size();
        averages.push_back(averages_si);
    }

    total_average/=total_number;
    return {averages,total_average};
}

bool cluster::convergence(vector<centroid> centroids_old)
{
    //Check if centroids converge by checking if they have the same
    //elements as previous assignment
    for (int i = 0; i < K; i++)
    {   
        if(centroids[i].vectors.size() != centroids_old[i].vectors.size())
                return false;
        sort(centroids_old[i].vectors.begin(),centroids_old[i].vectors.end());
        sort(centroids[i].vectors.begin(), centroids[i].vectors.end());

        if(centroids[i].vectors!=centroids_old[i].vectors) return false;
    }
    return true;
}

//Cluster Lloyd's
cluster_lloyds::cluster_lloyds(int K,vector<vector<float>> vectors) : cluster(K,vectors)
{
    //First assignment
    for(int i=0;i<n;i++)
    {
        centroid_item ci={p:vectors[i],index:i};
        float minimum=numeric_limits<float>::max();
        int minimum_index;
        for (int v=0;v<K;v++)
        {
            float distance=eucledian_distance(vectors[i],centroids[v].coordinates);
            if(distance<minimum)
            {
                minimum=distance;
                minimum_index= v;
            }
        }
        centroids[minimum_index].vectors.push_back(ci);
    }
    //Assignment/Update
    while(true)
    {
        vector<centroid> centroids_old=centroids;
        new_centroids();

        for(int i=0;i<n;i++)
        {
            centroid_item ci={p:vectors[i],index:i};
            float minimum=numeric_limits<float>::max();
            int minimum_index;
            for (int v=0;v<K;v++)
            {
                float distance=eucledian_distance(vectors[i],centroids[v].coordinates);
                if(distance<minimum)
                {
                    minimum=distance;
                    minimum_index= v;
                }
            }
            centroids[minimum_index].vectors.push_back(ci);
        }

        if(convergence(centroids_old)==true)
            break;
    }
}