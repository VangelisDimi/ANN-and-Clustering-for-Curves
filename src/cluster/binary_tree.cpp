#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<centroid_item> curves)
{
    Tree::n = curves.size();
    Tree::height = floor(log2(n));
    for(int i=0;i<n;i++)
        Tree::curves.push_back(curves.p)
    cout << "Constructing new Tree" << endl;
    placeChildren();
}

Tree::vector<vector<float>> postOrderTraversal(Node* node)
{
    vector<vector<float>> leftCurve;
    vector<vector<float>> rightCurve;
    if(isLeaf(node))
        return node->curve;
    else
        leftCurve = postOrderTraversal(node->leftChild);
        if (node->rightChild != NULL)
            rightCurve = postOrderTraversal(node->rightChild);
        return meanCurve(leftCurve, rightCurve);
}

Tree::bool isLeaf(Node* node)  
{
    if(node->leftChild == NULL && node->rightChild == NULL)
        return true; 
    else
        return false;
}

Tree::placeChildren()
{
    Node* currentNode = root;
    for(int i=0;i<height;i++)
    {
        
    }
}

