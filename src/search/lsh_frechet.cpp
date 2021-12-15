#include "lsh_frechet.hpp"
#include <math.h>
#include <numeric>
#include <map>
#include <fstream>
#include <chrono>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned int LSH_Frechet::g(vector<float> p,unsigned int j){
	return modulo(ID(p,j),tableSize);
}

unsigned long long int LSH_Frechet::ID(vector<float> p,unsigned int j) {
	unsigned long long int *h = new unsigned long long int[k];
	for(int i=0; i<k; i++)
		h[i] = modulo(hash_L2(i,p,v[j],t[j],w),M);
	unsigned long long int _g = 0;
	for(int i=0; i<k; i++)
		_g += modulo((r[i]*h[i]),M);
	delete[] h;
	return modulo(_g,M);
}

void LSH_Frechet::unmarkAssignedPoints()
{
	for(int p=0; p<L; p++)
		for(int id=0; id<tableSize;id++)
			for(auto it = hashtables[p].begin(id) ; it != hashtables[p].end(id) ; ++it)
			{
				(*it).flag=false;
				(*it).radius_found=0.0;
			}
}

vector<pair<float,unsigned int>> LSH_Frechet::find_N_nearest(vector<vector<float>> p,unsigned int N)
{
	//Returns indexes of N Nearest elements
	multimap<float, int> distances;
	for (int y=0 ; y<L ; y++)
	{
		vector<float> hash_vector=LSH_Frechet::prepare_curve(p,delta,vectorSize,t_snap[y]);
		unsigned long long int ID = LSH_Frechet::ID(hash_vector,y);
		for (auto it = hashtables[y].begin(ID); it != hashtables[y].end(ID); ++it )
		{	
			hashtable_item_lsh p_b = *it;
			if (p_b.ID == ID)
			{
				float distance = LSH_Frechet::distance(p,p_b.p);
				if(distances.find(distance) == distances.end() || distances.find(distance)->second != p_b.index)
					distances.insert({distance,p_b.index});
			}
		}
	}

	vector<pair<float,unsigned int>> N_Nearest;
	unsigned int y=0;
	auto it=distances.begin();
	for(;it != distances.end() && y<N ;++it,y++)
	{
		N_Nearest.push_back({it->first,it->second});
	}
	return N_Nearest;
}

vector<pair<float,unsigned int>> LSH_Frechet::find_R_nearest(vector<vector<float>> p,float R)
{
	//Returns indexes of R nearest element
	multimap<float, int> distances;
	for (int y=0 ; y<L ; y++)
	{
		vector<float> hash_vector=LSH_Frechet::prepare_curve(p,delta,vectorSize,t_snap[y]);
		unsigned long long int ID = LSH_Frechet::ID(hash_vector,y);
		for (auto it = hashtables[y].begin(ID); it != hashtables[y].end(ID); ++it )
		{
			hashtable_item_lsh p_b = *it;
			if (p_b.ID == ID)
			{
				if(clusterMode && p_b.flag && p_b.radius_found!=R) continue;
				float distance = LSH_Frechet::distance(p,p_b.p);
				if(distance<=R)
				{
					if(clusterMode)
					{
						p_b.flag = true;
						p_b.radius_found=R;
					}
					if(distances.find(distance) == distances.end() || distances.find(distance)->second != p_b.index)
						distances.insert({distance,p_b.index});
				}
			}
		}
	}

	vector<pair<float,unsigned int>> R_Nearest;
	auto it=distances.begin();
	for(;it != distances.end();++it)
	{
		R_Nearest.push_back({it->first,it->second});
	}

	return R_Nearest;
}

LSH_Frechet::LSH_Frechet(vector<vector<vector<float>>> input_curves,int k,int L,int metric,double delta,float hashtable_size_ratio)//Constructor
{
	//Initialize values
	LSH_Frechet::L=(metric!=CFD) ? L : 1 ;
	LSH_Frechet::k=k;
	LSH_Frechet::delta=delta;
	w=400;
	vectorSize=(!input_curves.empty()) ? input_curves[0].size() : 0;
	n=input_curves.size();
	tableSize=ceil(n*hashtable_size_ratio);
	hashtables = new hash_table<hashtable_item_lsh>[L];
	for (int i = 0; i < L; i++)
	{
		hashtables[i]=hash_table<hashtable_item_lsh>(tableSize);
	}
	
	v = new vector<float>*[L];
	t = new float*[L];

	for (int x = 0; x < k; x++)
	{
		r.push_back(uniform_distribution_rng(0,INT32_MAX));
	}
	for (int i = 0; i < L; i++)
	{
		v[i] = new vector<float>[k];
		t[i] = new float[k];
		for (int x = 0; x < k; x++)
		{
			t[i][x]=uniform_distribution_rng(0,w-1);
			for (int y = 0; y < vectorSize; y++)
			{
				v[i][x].push_back(normal_distribution_rng());
			}
		}
	}

	t_snap = new float*[L];
	if(metric==DFD)
	{
		distance=&getDiscreteFrechetDistance;
		prepare_curve=&TWO_DIM::prepareCurve;
		for(int i=0;i<L;i++)
		{
			t_snap[i]=new float[2];
			t_snap[i][0]=uniform_distribution_rng_float(0,delta-numeric_limits<float>::epsilon());
			t_snap[i][1]=uniform_distribution_rng_float(0,delta-numeric_limits<float>::epsilon());
		}
	}
	else if(metric==CFD)
	{
		distance=&continuousFrechetDistance;
		prepare_curve=&ONE_DIM::prepareCurve;
		for(int i=0;i<L;i++)
		{
			t_snap[i]=new float[1];
			t_snap[i][0]=uniform_distribution_rng_float(0,delta-numeric_limits<float>::epsilon());
		}
	}

	//Add vectors to L hashtables
	vector<float> vector1;
	vector<float> vector2;
	for(int i=0; i<n; i++)
	{
		for(int y=0;y<L;y++)
		{
			vector<float> hash_vector = LSH_Frechet::prepare_curve(input_curves[i],delta,vectorSize,t_snap[y]);
			hashtable_item_lsh p{input_curves[i],ID(hash_vector,y),i};
			hashtables[y].insert(p.ID,p);
		}
	}
	// cout << "disc " << getDiscreteFrechetDistance(input_curves[0],input_curves[1]) << endl;
	// cout << "cont " << continuousFrechetDistance(input_curves[0],input_curves[1]) << endl;
};

LSH_Frechet::~LSH_Frechet()//Destructor
{
	delete[] hashtables;
	for(int i=0;i<L;i++)
	{
		delete[] t[i];
		delete[] v[i];
		delete[] t_snap[i];
	}
	delete[] t;
	delete[] v;
	delete[] t_snap;
};