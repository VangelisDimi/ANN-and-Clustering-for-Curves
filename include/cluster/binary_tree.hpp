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
    vector<vector<float>> curves;
    int height = 0;
    int n = 0;

protected:
    Tree(vector<centroid_item> curves);
    ~Tree() {}
    placeChildren();
    vector<vector<float>> postOrderTraversal(Node* node);
    bool isLeaf(Node* node);
};

#endif