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
#include <list>
#include "utils.hpp"

#include "frechet.hpp"
#include "curve.hpp"

using namespace std;

namespace ONE_DIM
{

void filter(vector<vector<vector<float>>> &curves,double e)
{
	for(unsigned int i=0;i<curves.size();i++)
	{
		unsigned int y=1;
		while(y<curves[i].size()-1)
		{
			if(abs(curves[i][y+1][0]-curves[i][y][0])<e && abs(curves[i][y][0]-curves[i][y-1][0])<e)
			{
				curves[i].erase(curves[i].begin() + y);
				continue;
			}
			y++;
		}
	}
}

vector<vector<float>> snapCurve(vector<vector<float>> p, double delta, float t){
	vector<vector<float>> snappedCurve;
	for(int i=0; i<p.size(); i++){
		float snapped_y = floor((p[i][0]+t)/delta)*delta;
		snappedCurve.push_back({snapped_y});
	}
	return snappedCurve;
}

vector<float> concatCurve(vector<vector<float>> p,unsigned int vector_size){
	int curveLength = vector_size;

	int prev;
	int next;
	unsigned int i=1;
	while (i<p.size()-1)
	{
		prev=i-1;
		next=i+1;

		if(p[i][0]>=min(p[prev][0],p[next][0]) && p[i][0]<=max(p[prev][0],p[next][0]))
		{			
			p.erase(p.begin() + i);
			continue;
		}
		i++;
	}

	vector<float> concatedCurve;
	for(int i=0;i<p.size();i++)
	{
		concatedCurve.push_back(p[i][0]);
	}

	if( curveLength > concatedCurve.size() )
		for(int i=0; i<curveLength-concatedCurve.size(); i++)
			concatedCurve.push_back(std::numeric_limits<float>::max()-5);
	return concatedCurve;
}

vector<float> prepareCurve(vector<vector<float>> p, double delta,unsigned int vector_size,float *t){
	vector<vector<float>> snappedCurve = ONE_DIM::snapCurve(p, delta,t[0]);
	return ONE_DIM::concatCurve(snappedCurve,vector_size);
}

}

namespace TWO_DIM
{

void filter(vector<vector<float>> curve,double e)
{
	unsigned int y=1;
	while(y<curve.size()-1)
	{
		if(abs(curve[y+1][1]-curve[y][1])<e && abs(curve[y][1]-curve[y-1][1])<e)
		{
			curve.erase(curve.begin() + y);
			continue;
		}
		y++;
	}
}

vector<vector<float>> snapCurve(vector<vector<float>> p, double delta,float *t){
	float _x;
	float _y;
	float x;
	float y;
	vector<vector<float>> snappedCurve;
	for(int i=0; i<p.size(); i++){
		_x = p[i][0];
		_y = p[i][1];
		x  = floor((_x-t[0])/delta+(1/2))*delta + t[0];
		y  = floor((_y-t[1])/delta+(1/2))*delta + t[1];
		_x += 1;
		snappedCurve.push_back({x,y});
	}
	return snappedCurve;
}

vector<float> concatCurve(vector<vector<float>> p,unsigned int vector_size){
	//Remove duplicates
	unsigned int i=1;
	while(i<p.size())
	{
		float x = p[i][0];
		float y = p[i][1];
		float _x = p[i-1][0];
		float _y = p[i-1][1];
		if( x == _x && y == _y ){
			p.erase(p.begin() + i);
			continue;
		}
		i++;
	}

	//Concat
	int curveLength = vector_size*2;
	vector<float> concatedCurve;
	for(int i=0; i<p.size(); i++){
		float x = p[i][0];
		float y = p[i][1];
		concatedCurve.push_back(x);
		concatedCurve.push_back(y);
	}

	//Pad
	if( curveLength > concatedCurve.size() )
		for(int i=0; i<curveLength-concatedCurve.size(); i++)
			concatedCurve.push_back(std::numeric_limits<float>::max()-5);
	return concatedCurve;
}

vector<float> prepareCurve(vector<vector<float>> p, double delta,unsigned int vector_size,float *t){
	vector<vector<float>> snappedCurve = TWO_DIM::snapCurve(p, delta,t);
	return TWO_DIM::concatCurve(snappedCurve,vector_size);
}

}

Curve convertVectorToCurve(vector<vector<float>> p){
	Points points = Points(1);
	for(int i = 0 ; i < p.size() ; i++)
	{
		Point point = Point(1);
		point.set(0, p[i][0]);
		points.add(point);
	}
	return Curve(points);
}

float continuousFrechetDistance(vector<vector<float>> p, vector<vector<float>> q)
{
	return Frechet::Continuous::distance(convertVectorToCurve(p), convertVectorToCurve(q)).value;
}

float getDiscreteFrechetDistance(vector<vector<float>> p, vector<vector<float>> q)
{
	float c[p.size()][q.size()];
	for(int i=0;i<p.size();i++)
	{
		for(int j=0;j<q.size();j++)
		{
			if(i==0 && j == 0)
				c[0][0] = eucledian_distance(p[0],q[0]);
			else if(i==0 && j>0)
				c[i][j] = max(c[0][j-1],eucledian_distance(p[0],q[j]));
			else if(i>0 && j==0)
				c[i][j] = max(c[i-1][0],eucledian_distance(p[i],q[0]));
			else
				c[i][j] = max(min({c[i-1][j],c[i-1][j-1],c[i][j-1]}),eucledian_distance(p[i],q[j]));
		}
	}
	return c[p.size()-1][q.size()-1];
}

vector<vector<float>> MeanCurve(vector<vector<float>> p, vector<vector<float>> q)
{
	float c[p.size()][q.size()];
	for(int i=0;i<p.size();i++)
	{
		for(int j=0;j<q.size();j++)
		{
			if(i==0 && j == 0)
				c[0][0] = eucledian_distance(p[0],q[0]);
			else if(i==0 && j>0)
				c[i][j] = max(c[0][j-1],eucledian_distance(p[0],q[j]));
			else if(i>0 && j==0)
				c[i][j] = max(c[i-1][0],eucledian_distance(p[i],q[0]));
			else
				c[i][j] = max(min({c[i-1][j],c[i-1][j-1],c[i][j-1]}),eucledian_distance(p[i],q[j]));
		}
	}

	list<pair<int,int>> traversal;
	int Pi=p.size()-1,Qi=q.size()-1;
	traversal.push_front({Pi,Qi});
	while (Pi!=0 && Qi!=0)
	{
		vector<float> idxvec={c[Pi-1][Qi],c[Pi][Qi-1],c[Pi-1][Qi-1]};
		int minidx=1;
		for(int i=1;i<3;i++)
			if(idxvec[i]<idxvec[minidx]) minidx=i;
		if(minidx==0)
			traversal.push_front({--Pi,Qi});
		else if(minidx==1)
			traversal.push_front({Pi,--Qi});
		else
			traversal.push_front({--Pi,--Qi});
	}

	vector<vector<float>> meanvec;
	for (auto it = traversal.begin(); it != traversal.end(); ++it)
	{
		int i=it->first;
		int j=it->second;
		meanvec.push_back({(p[i][0]+q[j][0])/2,(p[i][1]+q[j][1])/2});
	}
	return meanvec;
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

float uniform_distribution_rng_float(float lowerRange,float higherRange)
{
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_real_distribution<float> distr(lowerRange, higherRange);

	return distr(generator);
}

void read_file_vector(string filename,vector<vector<float>> &vectors,vector<string> &ids)
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

void read_file_curve(string filename,vector<vector<vector<float>>> &curves,vector<string> &ids,unsigned int dimensions)
{
	ifstream  file(filename);
	unsigned int filelines=count(istreambuf_iterator<char>(file), 
    istreambuf_iterator<char>(), '\n');

	curves=vector<vector<vector<float>>>(filelines);

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
		int y=0;
		while(lineStream >> value)
		{
			vector<float> point;
			if(dimensions==2)
				point.push_back(y);
			point.push_back(value);
			curves[i].push_back(point);
			y++;
		}
		i++;
	}
}