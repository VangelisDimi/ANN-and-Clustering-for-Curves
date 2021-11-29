#pragma once
#ifndef utils_h
#define utils_h

using namespace std;
#include <string>
#include <map>

#include "exhaustive_search.hpp"


float eucledian_distance(vector<float>,vector<float>);
void read_file(string filename,vector<vector<float>> &vectors,vector<string> &ids);
float normal_distribution_rng();
int uniform_distribution_rng(int lowerRange,int higherRange);
int uniform_distribution_rng_float(float lowerRange,float higherRange);
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
void write_file(ofstream &outfile,string query_id,vector<string> ids,vector<pair<float,unsigned int>> N_Nearest,vector<pair<float,unsigned int>> R_Nearest,vector<pair<float,unsigned int>> True_N_Nearest,double time,double time_true,string algorithm);

#endif