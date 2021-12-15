#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<centroid_item> curves)
{
    Tree::n = curves.size();
    Tree::height = floor(log2(n));
    cout << "Constructing new Tree" << endl;
    Node* currentNode = root;
    for(int i=0;i<height;i++)
    {
        
    }
    postOrderTraversal(root);
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

Tree::vector<vector<float>> getMeanCurve()
{
    return root->curve;
}

