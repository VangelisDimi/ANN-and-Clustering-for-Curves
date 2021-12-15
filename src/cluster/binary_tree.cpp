#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>

vector<vector<float>> postOrderTraversal(Tree::Node* node)
{
    vector<vector<float>> leftCurve;
    vector<vector<float>> rightCurve;
    if(isLeaf(node))
        return node->curve;
    else
        leftCurve = postOrderTraversal(node->leftChild);
        if (node->rightChild == NULL)
            rightCurve = postOrderTraversal(node->rightChild);
        // else
        //     rightCurve = NULL;
        return meanCurve(leftCurve, rightCurve);
}


bool isLeaf(Tree::Node* node) {
    if(node->leftChild == NULL && node->rightChild == NULL)
        return true; 
    else
        return false;
}

