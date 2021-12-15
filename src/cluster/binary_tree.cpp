#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>

Tree::Tree()
{
    cout << "Constructing new Tree" << endl;
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

Tree::bool isLeaf(Node* node) {
    if(node->leftChild == NULL && node->rightChild == NULL)
        return true; 
    else
        return false;
}

