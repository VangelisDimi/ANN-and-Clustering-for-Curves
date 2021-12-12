#include "cluster_ANN.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>

#define FLT_MAX 3.40282e+038

cluster_ANN::cluster_ANN(int K,vector<vector<float>> vectors) : cluster(K,vectors)
{
    cluster_ANN::vectors=vectors;
}

//cluster_ANN
void cluster_ANN::bruteforce_assignment(vector<tuple<int,int,float>> flagged_indexes)
{
    int found = 0;
    for(int i=0;i< cluster_ANN::vectors.size();i++)
    {
        for(int j=0;j< flagged_indexes.size();j++)
            if(get<0>(flagged_indexes[j])==i){
                found = 1;
            }
        if(found==1){
            found=0;
            continue;
        }
        centroid_item ci={p:vectors[i],index:i};
        float minimum=numeric_limits<float>::max();
        int minimum_index;
        for (int v=0;v< cluster::centroids.size();v++)
        {
            float distance=eucledian_distance(cluster_ANN::vectors[i],cluster::centroids[v].coordinates);
            if(distance<minimum)
            {
                minimum=distance;
                minimum_index= v;
            }
        }
        centroids[minimum_index].vectors.push_back(ci);
    }
}

float cluster_ANN::init_search_radius()
{
    float min_distance=numeric_limits<float>::max();
    float current_distance;
    for(int i=0;i<K;i++)
        for(int j=i+1;j<K;j++){
            current_distance = eucledian_distance(cluster::centroids[i].coordinates, cluster::centroids[j].coordinates);
            if(current_distance<min_distance)
                min_distance = current_distance;
        }
    return min_distance/2;
}

bool cluster_ANN::terminationCriterion(float search_radius, int* updatedCentroid)
{
    if(search_radius>(std::numeric_limits<float>::max()/2)+5 || search_radius==0)
        return true;
    int check = 0;
    for(int i=0;i<K;i++){
        if(updatedCentroid[i]>0)
            check++;
        updatedCentroid[i] = 0;
    }
    if(check<=K/2)
        return true;
    return false;
}

//Cluster LSH
cluster_lsh::cluster_lsh(vector<vector<float>> vectors,int K,int k,int L) : cluster_ANN(K,vectors), LSH(vectors,k,L,L2,0.125)
{
    LSH::clusterMode = true;
    //First assignment
    new_assignment();

    //Assignment/Update
    while(true)
    {
        vector<centroid> centroids_old=centroids;
        new_centroids();
        new_assignment();

        if(convergence(centroids_old)==true)
            break;
    }
}

void cluster_lsh::new_assignment()
{
    vector<tuple<int,int,float>> flagged_indexes;

    float search_radius = init_search_radius();

    rangeSearch_Assignment(flagged_indexes, search_radius);

    cluster_ANN::bruteforce_assignment(flagged_indexes);

    LSH::unmarkAssignedPoints();
}

void cluster_lsh::rangeSearch_Assignment(vector<tuple<int,int,float>> flagged_indexes, float search_radius)
{
    int updatedCentroid[K];
    for(int i=0;i<K;i++)
        updatedCentroid[i] = 0;

    while(true){
        vector<tuple<int,int,float>> new_indexes;
        int found = 0;

        for(int i=0;i<K;i++){
            vector<pair<float,unsigned int>> R_Nearest = LSH::find_R_nearest(cluster::centroids[i].coordinates, search_radius);
            for(int j=0;j< R_Nearest.size();j++)
            {
                int Index = get<unsigned int>(R_Nearest[j]);
                found = 0;
                for(int l=0;l< flagged_indexes.size();l++)
                {
                    if(get<0>(flagged_indexes[l])==Index)
                        found = 1;
                }
                if(found!=1){      
                    centroid_item ci={p:cluster_ANN::vectors[Index],index:Index};
                    cluster::centroids[i].vectors.push_back(ci);
                    new_indexes.push_back({Index,i,get<float>(R_Nearest[j])});
                    updatedCentroid[i]++;
                }
            }
        }
        for(int i=0;i< new_indexes.size();i++)
        {
            int indexi = get<0>(new_indexes[i]);
            int centroidi = get<1>(new_indexes[i]);
            float distancei = get<float>(new_indexes[i]);

            for(int j=0;j< new_indexes.size();j++)
            {
                int indexj = get<0>(new_indexes[j]);
                int centroidj = get<1>(new_indexes[j]);
                float distancej = get<float>(new_indexes[j]);
                if(indexi==indexj && centroidi!=centroidj)
                {
                    if(distancei<=distancej)
                    {
                        for(int k=0;k< cluster::centroids[centroidj].vectors.size();k++)
                        {
                            if(centroids[centroidj].vectors[k].index==indexj){
                                centroids[centroidj].vectors.erase(centroids[centroidj].vectors.begin()+k);
                                new_indexes.erase(new_indexes.begin()+j);
                                updatedCentroid[centroidj]--;
                            }
                        }
                    }
                    else               
                    {
                        for(int k=0;k< cluster::centroids[centroidi].vectors.size();k++)
                        {
                            if(centroids[centroidi].vectors[k].index==indexi){
                                centroids[centroidi].vectors.erase(centroids[centroidi].vectors.begin()+k);
                                new_indexes.erase(new_indexes.begin()+i);
                                updatedCentroid[centroidi]--;
                            }
                        }
                    }
                }
            }
        }
        for(int i=0;i< new_indexes.size();i++)
            flagged_indexes.push_back(new_indexes[i]);
        
        if(terminationCriterion(search_radius, updatedCentroid))
            break;
        search_radius = search_radius*2;
    }
}

//Cluster hypercube
cluster_cube::cluster_cube(vector<vector<float>> vectors,int K,int k,int probes,int M) : cluster_ANN(K,vectors), Cube(vectors,k,M,probes,L2)
{
    Cube::clusterMode = true;
    
    //First assignment
    new_assignment();

    //Assignment/Update
    while(true)
    {
        vector<centroid> centroids_old=centroids;
        new_centroids();
        new_assignment();

        if(convergence(centroids_old)==true)
            break;
    }
}

void cluster_cube::new_assignment()
{
    vector<tuple<int,int,float>> flagged_indexes;

    float search_radius = init_search_radius();

    rangeSearch_Assignment(flagged_indexes, search_radius);

    cluster_ANN::bruteforce_assignment(flagged_indexes);

    Cube::unmarkAssignedPoints();
}

void cluster_cube::rangeSearch_Assignment(vector<tuple<int,int,float>> flagged_indexes, float search_radius)
{
    int updatedCentroid[K];
    for(int i=0;i<K;i++)
        updatedCentroid[i] = 0;

    while(true){
        vector<tuple<int,int,float>> new_indexes;
        int found = 0;
        search_radius = search_radius*2;
        for(int i=0;i<K;i++){
            vector<pair<float,unsigned int>> R_Nearest = Cube::find_R_nearest(cluster::centroids[i].coordinates, search_radius);
            for(int j=0;j< R_Nearest.size();j++)
            {
                int Index = get<unsigned int>(R_Nearest[j]);
                found = 0;
                for(int l=0;l< flagged_indexes.size();l++)
                {
                    if(get<0>(flagged_indexes[l])==Index)
                        found = 1;
                }
                if(found!=1){           
                    centroid_item ci={p:cluster_ANN::vectors[Index],index:Index};
                    cluster::centroids[i].vectors.push_back(ci);
                    new_indexes.push_back({Index,i,get<float>(R_Nearest[j])});
                    updatedCentroid[i]++;
                }
            }
        }
        for(int i=0;i< new_indexes.size();i++)
        {
            int indexi = get<0>(new_indexes[i]);
            int centroidi = get<1>(new_indexes[i]);
            float distancei = get<float>(new_indexes[i]);
            for(int j=0;j< new_indexes.size();j++)
            {
                int indexj = get<0>(new_indexes[j]);
                int centroidj = get<1>(new_indexes[j]);
                float distancej = get<float>(new_indexes[j]);
                if(indexi==indexj && centroidi!=centroidj)
                {
                    if(distancei<=distancej)
                    {
                        for(int k=0;k< cluster::centroids[centroidj].vectors.size();k++)
                        {
                            if(centroids[centroidj].vectors[k].index==indexj){
                                centroids[centroidj].vectors.erase(centroids[centroidj].vectors.begin()+k);
                                new_indexes.erase(new_indexes.begin()+j);
                                updatedCentroid[centroidj]--;
                            }
                        }
                    }
                    else               
                    {
                        for(int k=0;k< cluster::centroids[centroidi].vectors.size();k++)
                        {
                            if(centroids[centroidi].vectors[k].index==indexi){
                                centroids[centroidi].vectors.erase(centroids[centroidi].vectors.begin()+k);
                                new_indexes.erase(new_indexes.begin()+i);
                                updatedCentroid[centroidi]--;
                            }
                        }
                    }
                }
            }
        }
        for(int i=0;i< new_indexes.size();i++)
            flagged_indexes.push_back(new_indexes[i]);
        
        if(terminationCriterion(search_radius, updatedCentroid))
            break;
    }
}