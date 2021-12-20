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

class Node
{
public:
    vector<vector<float>> curve;
    Node* leftChild;
    Node* rightChild;

protected:
    Node(): leftChild(NULL), rightChild(NULL) {}
    Node(vector<vector<float>> curve): curve(curve), leftChild(NULL), rightChild(NULL) {}
    Node(Node* leftChild, Node* rightChild): leftChild(leftChild), rightChild(rightChild) {}
    Node(vector<vector<float>> curve, Node* leftChild, Node* rightChild): curve(curve), leftChild(leftChild), rightChild(rightChild) {}
    ~Node() {}
};

#endif