#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<centroid_item> curves)
{
    Tree::n = curves.size();
    Tree::height = floor(log2(n));
    Tree::curveSize = curves[0].p.size();
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
        vector<vector<float>> meancurve=meanCurve(leftCurve, rightCurve);
        double e=0.1;
        while(meancurve.size()>curveSize)
        {
            TWO_DIM::filter(meancurve,e);
            e*=2;
        }
        return meanCurve;
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
    Node** parents;
    for(int i=0;i<nParents;i++)
        parents[i] = new Node;
}

