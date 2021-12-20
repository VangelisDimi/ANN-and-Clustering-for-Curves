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
    Node* root;
    Node** children;
    vector<vector<vector<float>>> curves;
    int height = 0;
    int n = 0;
    int curveSize;
    Tree(vector<cluster_Frechet::centroid_item> curves);
    ~Tree() {}
    vector<vector<float>> postOrderTraversal(Node* node);

protected:

    void placeChildren();
    bool isLeaf(Node* node);
};

class Node
{
public:
    vector<vector<float>> curve;
    Node* leftChild;
    Node* rightChild;
    Node(): leftChild(NULL), rightChild(NULL) {}
    Node(vector<vector<float>> curve): curve(curve), leftChild(NULL), rightChild(NULL) {}
    Node(vector<vector<float>> curve, Node* leftChild, Node* rightChild): curve(curve), leftChild(leftChild), rightChild(rightChild) {}
    ~Node() {}
protected:

};


#endif