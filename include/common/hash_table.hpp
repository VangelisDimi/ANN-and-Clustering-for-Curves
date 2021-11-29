#pragma once
#ifndef hash_table_h
#define hash_table_h
//HashTable of fixed size with overflow buckets
//Created using vectors
#include <vector>
#include <list>
using namespace std;

template <typename V>
class hash_table
{
private:
    vector<vector<V>> Buckets;
    unsigned int tablesize;
public:
    hash_table(unsigned int size=28083486)
    {
        hash_table::Buckets = vector<vector<V>>(size);
        hash_table::tablesize=size;
    };

    void insert(unsigned int key,V value)
    {
        Buckets[key%tablesize].push_back(value);
    };

    typename vector<V>::iterator begin(unsigned int key)
    {
        return Buckets[key%tablesize].begin();
    };

    typename vector<V>::iterator end(unsigned int key)
    {
        return Buckets[key%tablesize].end();
    };
};

#endif