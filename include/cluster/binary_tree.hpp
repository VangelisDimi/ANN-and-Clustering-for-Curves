#pragma once
#ifndef binary_tree_h
#define binary_tree_h

using namespace std;
#include <vector>
#include <list>
#include "cluster_f.hpp"

class Node
{
public:
    vector<vector<float>> curve;
    Node* leftChild;
    Node* rightChild;
    Node(): leftChild(NULL), rightChild(NULL) {}
    Node(vector<vector<float>> curve): curve(curve), leftChild(NULL), rightChild(NULL) {}
    Node(Node* leftChild, Node* rightChild): leftChild(leftChild), rightChild(rightChild) {}
    Node(vector<vector<float>> curve, Node* leftChild, Node* rightChild): curve(curve), leftChild(leftChild), rightChild(rightChild) {}
    ~Node() {}
protected:

};


class Tree
{
public:
    Node*** structure;
    Node* root;
    Node** children;
    vector<int> floors;
    vector<vector<vector<float>>> curves;
    int height = 0;
    int n = 0;
    int mycounter=0;
    int curveSize;
    bool error = false;
    Tree(vector<cluster_Frechet::centroid_item> curves);
    ~Tree();
    vector<vector<float>> postOrderTraversal(Node* node);
    Node* createNode(int level);

protected:

    void placeChildren();
    bool isLeaf(Node* node);
};


#endif