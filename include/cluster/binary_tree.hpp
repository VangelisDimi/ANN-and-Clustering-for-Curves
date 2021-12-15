#pragma once
#ifndef binary_tree_h
#define binary_tree_h

using namespace std;
#include <vector>
#include <list>

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

protected:
    Tree();
    vector<vector<float>> postOrderTraversal(Node node);
    bool isLeaf(Node node);

};

#endif