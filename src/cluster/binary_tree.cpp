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
    int nChildren = n;
    int nParents = ceil(nChildren/2.0);
    for(int i=0;i<n;i++)
        children[i] = new Node(curves[i]);
    while(1){
        int counter = 0;
        Node** parents;
        Node** children = Tree::children;
        Node* childx = NULL;
        Node* childy = NULL;
        for(int i=0;i<nParents;i++)
        {
            if (counter<nChildren)
                childx = children[counter++];
            else
                childx = NULL;
            if (counter<nChildren)
                childy = children[counter++];
            else
                childy = NULL;
            parents[i] = new Node(childx, childy);
        }
        nChildren = nParents;
    }
}

