#pragma once
#ifndef binary_tree_h
#define binary_tree_h

using namespace std;
#include <vector>
#include <list>
#include "cluster_f.hpp"

class Tree
{
public:
    typedef struct Node
    {
        vector<vector<float>> curve;
        Node* leftChild = NULL;
        Node* rightChild = NULL;
    }
    Node;
    Node* root;
    int height = 0;
    int n = 0;

protected:
    Tree(int n, vector<centroid_item>);
    vector<vector<float>> postOrderTraversal(Node* node);
    bool isLeaf(Node* node);
    vector<vector<float>> getMeanCurve();
};

#endif