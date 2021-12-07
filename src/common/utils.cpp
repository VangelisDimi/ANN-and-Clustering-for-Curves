//File for program utilities
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>    
#include "utils.hpp"

// #include "frechet.hpp"
// #include "curve.hpp"

using namespace std;

namespace ONE_DIM
{

vector<float> snapCurve(vector<float> p, double delta){
	vector<float> snappedCurve;
	for(int i=0; i<p.size(); i++){
		snappedCurve.push_back(delta*floor(p[i] + delta/2));
	}
	return snappedCurve;
}

}

namespace TWO_DIM
{

vector<vector<float>> snapCurve(vector<float> p, double delta){
	float _x = 0.0;
	float _y = 0.0;
	float x = 0.0;
	float y = 0.0;
	vector<vector<float>> snappedCurve;
	for(int i=0; i<p.size(); i++){
		_y = p[i];
		x  = delta*floor(_x + delta/2);
		y  = delta*floor(_y + delta/2);
		_x += 1;
		snappedCurve.push_back({x,y});
	}
	return snappedCurve;
}

vector<float> concatCurve(vector<vector<float>> p){
	vector<float> concatedCurve;
	float _x = p[0][0];
	float _y = p[0][1];
	float x = p[0][0];
	float y = p[0][1];
	int curveLength = p.size()*2;
	for(int i=0; i<p.size(); i++){
		x = p[i][0];
		y = p[i][1];
		if( x == _x && y == _y ){
			_x = x;
			_y = y;
			continue;
		}
		concatedCurve.push_back(x);
		concatedCurve.push_back(y);
		_x = x;
		_y = y;
	}
	if( curveLength > concatedCurve.size() )
		for(int i=0; i>curveLength-concatedCurve.size(); i++)
			concatedCurve.push_back(std::numeric_limits<float>::max()-5);
	return concatedCurve;
}

vector<float> prepareCurve(vector<float> p, double delta){
	vector<vector<float>> snappedCurve = TWO_DIM::snapCurve(p, delta);
	return TWO_DIM::concatCurve(snappedCurve);
}

}


// float continuousFrechetDistance(vector<float> p, vector<float> q)
// {
// 	Frechet::Continuous::Distance distance = Frechet::Continuous::distance(new Curve(p), new Curve(q));
// 	return distance.value;
// }


float discreteFrechetDistance(vector<vector<float>> p, vector<vector<float>> q)
{
	unsigned int i = p.size();
	unsigned int j = q.size();
	return getDiscreteFrechetDistance(p, q, i, j);
}

float getDiscreteFrechetDistance(vector<vector<float>> p, vector<vector<float>> q,unsigned int i,unsigned int j)
{
	if(i==1 && j == 1)
		return eucledian_distance(p[1],q[1]);
	else if(i==1 && j>1)
		return max(getDiscreteFrechetDistance(p,q,1,j-1),eucledian_distance(p[1],q[j]));
	else if(i>1 && j==1)
		return max(getDiscreteFrechetDistance(p,q,i-1,1),eucledian_distance(p[i],q[1]));
	else
		return max(min({getDiscreteFrechetDistance(p,q,i-1,j),getDiscreteFrechetDistance(p,q,i-1,j-1),getDiscreteFrechetDistance(p,q,i,j-1)}),eucledian_distance(p[i],q[j]));
}


vector<int> getNearbyProbes(int key, int n, int size)
{
	string strkey = fixedDecimalToBinary(key, size);
	int len = strkey.length(); 
	int counter = 1;
	int replaced = 0;
	int hammingDistance = 1;
	vector<string> _nearbyProbes;
	_nearbyProbes.push_back(strkey);
	while(counter<n){
		string probe = strkey;
		for(int i=0; i<len; i++){
			if(probe[i] == '0')
				probe[i]='1';
			else if(probe[i] == '1')
				probe[i]='0';
			replaced++;
			if(replaced==hammingDistance && counter<=n){
				if(find(_nearbyProbes.begin(), _nearbyProbes.end(), probe)==_nearbyProbes.end()){
					_nearbyProbes.push_back(probe);
					counter++;
					probe = strkey;
				}
				replaced = 0;
			}
			else if(counter>n)
				break;
		}
		hammingDistance++;
	}

	int _size = _nearbyProbes.size();
	vector<int> nearbyProbes;
	for(int i=0; i<_size; i++)
		nearbyProbes.push_back(binaryToDecimal(_nearbyProbes[i]));
	return nearbyProbes;
}

char intToChar(int number)
{
	string str = to_string(number);
	char const *num_char = str.c_str();
	return num_char[0];
}

int getHammingDistance(string first, string second)
{
	int len1 = first.size();
	int len2 = second.size();
	int _hammingDistance = 0;
	for(int i=0; i<min(len1, len2); i++){
		if(first[i] != second[i]){
			string j(1,first[i]);
			string k(1,second[i]);
			_hammingDistance += abs(stoi(j)-stoi(k));
		}
	}
	if(len1 != len2)
		_hammingDistance += abs(len1-len2);
	return _hammingDistance;
}

string decimalToBinary(int decimal) 
{
    string binary;
    while(decimal != 0) 
	{
		binary = ( decimal % 2 == 0 ? "0" : "1" ) + binary; 
		decimal /= 2;
	}
    return binary;
}

string fixedDecimalToBinary(int decimal, int size) 
{
    string binary = decimalToBinary(decimal);
	string zeros;
	int len = binary.length();
	for(int i=0; i<size-len; i++)
		zeros.push_back('0');
	return zeros.append(binary);
}

int binaryToDecimal(string binary){
	int decimal = stoi(binary, 0, 2);
	return decimal;
}

int getValue(map<int,int> m, int key){
	return m.find(key)->second;
}

int notExists(map<int,int> m, int key){
	if(m.find(key) == m.end())
		return 1;
	else 
		return 0;
}

float eucledian_distance(vector<float> p1,vector<float> p2)
{
	float distance=0;
	for (unsigned int i=0;i<p1.size();i++)
	{
		distance+=pow((p2[i]-p1[i]),2);
	}
	return sqrt(distance);
}

unsigned long long int modulo(long long int a,long long int b)
{
	//Real modulo operator
	const int result = a % b;
	return result >= 0 ? result : result + b;
}

float normal_distribution_rng()
{
	//Random number generator using normal distribution
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<float> d(0,1);
	return d(gen);
}

int uniform_distribution_rng(int lowerRange,int higherRange)
{
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_int_distribution<int> distr(lowerRange, higherRange);

	return distr(generator);
}  

int uniform_distribution_rng_float(float lowerRange,float higherRange)
{
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_real_distribution<float> distr(lowerRange, higherRange);

	return distr(generator);
}

void read_file(string filename,vector<vector<float>> &vectors,vector<string> &ids)
{
	ifstream  file(filename);
	unsigned int filelines=count(istreambuf_iterator<char>(file), 
    istreambuf_iterator<char>(), '\n');

	vectors=vector<vector<float>>(filelines);

	file.clear();
	file.seekg(0,ios::beg);
	string  line;
	int i=0;
	while(getline(file, line))
	{
		stringstream  lineStream(line);

		float value;
		string id;
		lineStream >> id;
		ids.push_back(id);//Vector Id
		while(lineStream >> value)
		{
			vectors[i].push_back(value);
		}
		i++;
	}
}