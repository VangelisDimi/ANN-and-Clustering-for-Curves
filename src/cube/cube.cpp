#include "cube.hpp"
#include <math.h>
#include <numeric>
#include <map>
#include <utils.hpp>
#include <unordered_map>

#define M 4294967291

unsigned int Cube::f(int i, int h_p) {
	int _f = rand() % 2;
	if (notExists(f_table[i], h_p)) {
		f_table[i].insert({h_p, _f});
		return _f;
	} 
	else 
		return getValue(f_table[i], h_p);
}

unsigned int Cube::F(vector<float> p){
	string _F;
	char c;
	for(int i=0; i<k; i++){
		c = intToChar(f(i,hash_L2(i,p,v,t,w)));
		_F.push_back(c);
	}
	return binaryToDecimal(_F);
}

vector<pair<float,unsigned int>> Cube::find_N_nearest(vector<float> p,unsigned int N)
{
	//Returns indexes of N Nearest elements
	multimap<float, int> distances;
    int counter = 0;
	vector<int> nearbyProbes = getNearbyProbes(F(p), probes, k);

	for(int i=0; i<probes; i++)
		for (auto it = hypercube->begin(nearbyProbes[i]); it != hypercube->end(nearbyProbes[i]); ++it )
		{
			if(counter >= argM)
				break;
			hashtable_item_cube p_b = *it;
			float distance = Cube::distance(p,p_b.p);
			distances.insert({distance,p_b.index});
			counter++;
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

void Cube::unmarkAssignedPoints()
{
	for(int i=0; i<pow(2,k);i++)
	{
		for(auto it = hypercube->begin(i) ; it != hypercube->end(i) ; ++it){
			(*it).flag=false;
			(*it).radius_found=0.0;
		}
	}
}

vector<pair<float,unsigned int>> Cube::find_R_nearest(vector<float> p,float R)
{
	//Returns indexes of R nearest element
	multimap<float, int> distances;
    int counter = 0;

	vector<int> nearbyProbes = getNearbyProbes(F(p), probes, k);
	for(int i=0; i<probes; i++)
	{
		for (auto it = hypercube->begin(nearbyProbes[i]); it != hypercube->end(nearbyProbes[i]); ++it )	
		{
			if(counter >= argM)
				break;
			hashtable_item_cube p_b = *it;
			if(clusterMode && p_b.flag && p_b.radius_found!=R) continue;
			float distance = Cube::distance(p,p_b.p);
			if(distance<=R)
			{
				if(clusterMode)
				{
					p_b.flag = true;
					p_b.radius_found = R;
				}
				distances.insert({distance,p_b.index});
			}
			counter++;
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

Cube::Cube(vector<vector<float>> input_vectors, int k, int argM, int probes, int metric)
{
	//Initialize values
	Cube::k=k;
	Cube::argM=argM;
	Cube::probes=probes;
	vectorSize=(!input_vectors.empty()) ? input_vectors[0].size() : 0;
	f_table = new map<int, int>[k];
	hypercube = new hash_table<hashtable_item_cube>(pow(2,k));
	w=300;
	v = new vector<float>[k];
	t = new float[k];

	for (int i = 0; i < k; i++)
	{
		for (int y = 0; y < vectorSize; y++)
		{
			v[i].push_back(normal_distribution_rng());
		}
		t[i] = uniform_distribution_rng(0,w-1);
	}

	if(metric==L2)
		distance=&eucledian_distance;

	for(int i = 0;i<(int) input_vectors.size();i++)
	{
		hashtable_item_cube p{input_vectors[i],i};
		hypercube->insert(F(input_vectors[i]),p);
	}
};

Cube::~Cube()//Destructor
{
	delete[] v;
	delete[] t;
	delete[] f_table;
	delete hypercube;
};
