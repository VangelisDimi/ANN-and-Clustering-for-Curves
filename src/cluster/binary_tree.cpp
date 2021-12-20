#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<cluster_Frechet::centroid_item> OGcurves)
{
    Tree::n = OGcurves.size();
    Tree::height = floor(log2(n));
    Tree::curveSize = OGcurves[0].p.size();
    for(int i=0;i<n;i++)
        Tree::curves.push_back(OGcurves[i].p);
    cout << "Constructing new Tree" << endl;
    placeChildren();
}

vector<vector<float>> Tree::postOrderTraversal(Node* node)
{
    cout << "postOrderTraversal" << node->curve << endl;
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
        return meancurve;
}

bool Tree::isLeaf(Node* node)  
{
    if(node->leftChild == NULL && node->rightChild == NULL)
        return true; 
    else
        return false;
}

void Tree::placeChildren()
{
    vector<int> floors;
    while(1){
        cout << n << endl;
        floors.push_back(n);
        n = ceil(n/2.0);
        if(n==1){
            floors.push_back(1);
            break;
        }
    }
    Node*** structure = new Node**[floors.size()];
    Node* childx = NULL;
    Node* childy = NULL;
    for(int i=0;i<floors.size();i++){
        structure[i] = new Node*[floors[i]];
        if(i==0)
            for(int j=0;j<curves.size();j++)
                structure[i][j] = new Node(curves[j]);
        else
            for(int j=0;j<floors[i]-1;j=j+2){
                structure[i][j] = new Node(structure[i-1][j], structure[i-1][j+1]);
            }
        cout<<i<<" "<<floors[i]<<endl;
    }
}

