#pragma once
#ifndef utils_h
#define utils_h

using namespace std;
#include <string>
#include <map>
#include "frechet.hpp"
#include "curve.hpp"
#include "exhaustive_search.hpp"

namespace ONE_DIM{
    void filter(vector<vector<vector<float>>> &curves,double e=1);
    vector<vector<float>> snapCurve(vector<vector<float>> p, double delta,float t);
    vector<float> concatCurve(vector<vector<float>> p,unsigned int vector_size);
    vector<float> prepareCurve(vector<vector<float>> p, double delta,unsigned int vector_size,float *t);
}
namespace TWO_DIM{
    void filter(vector<vector<float>> curve,double e=1);
    vector<vector<float>> snapCurve(vector<vector<float>> p, double delta,float *t);
    vector<float> concatCurve(vector<vector<float>> p,unsigned int vector_size);
    vector<float> prepareCurve(vector<vector<float>> p, double delta,unsigned int vector_size,float *t);
}
Curve convertVectorToCurve(vector<vector<float>> p);
float continuousFrechetDistance(vector<vector<float>> p, vector<vector<float>> q);
float getDiscreteFrechetDistance(vector<vector<float>> p, vector<vector<float>> q);
vector<vector<float>> MeanCurve(vector<vector<float>> p, vector<vector<float>> q);

float eucledian_distance(vector<float>,vector<float>);
float normal_distribution_rng();
int uniform_distribution_rng(int lowerRange,int higherRange);
float uniform_distribution_rng_float(float lowerRange,float higherRange);
unsigned long long int modulo(long long int a,long long int b);
float inner_product(vector <float> v1,vector <float> v2);
int getHammingDistance(string first, string second);
string decimalToBinary(int decimal);
int binaryToDecimal(string binary);
char intToChar(int number);
int getValue(map<int,int> m, int key);
int notExists(map<int,int> m, int key);
vector<int> getNearbyProbes(int key, int n, int size);
string fixedDecimalToBinary(int decimal, int size);
void read_file_vector(string filename,vector<vector<float>> &vectors,vector<string> &ids);
void read_file_curve(string filename,vector<vector<vector<float>>> &curves,vector<string> &ids,unsigned int dimensions);

#endif