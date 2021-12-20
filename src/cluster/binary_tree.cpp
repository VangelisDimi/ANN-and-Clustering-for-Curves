#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     

Tree::Tree(vector<cluster_Frechet::centroid_item> OGcurves)
{
    Tree::n = OGcurves.size();
    Tree::height = floor(log2(n));
    Tree::root = NULL;
    if(OGcurves.size()>0)
        Tree::curveSize = OGcurves[0].p.size();
    else{
        error = true;
        return;
    }
    for(int i=0;i<n;i++)
        Tree::curves.push_back(OGcurves[i].p);
    cout << "Constructing new Tree" << endl;
    placeChildren();
}

vector<vector<float>> Tree::postOrderTraversal(Node* node)
{
    // cout << "postOrderTraversal" << endl;
    vector<vector<float>> leftCurve;
    vector<vector<float>> rightCurve;
    if(isLeaf(node)){
        // cout << "postOrderTraversal1" << endl;
        return node->curve;
    }
    else
    {
        // cout << "postOrderTraversal2" << endl;
        leftCurve = postOrderTraversal(node->leftChild);
        // cout << "postOrderTraversal3" << endl;
        if (node->rightChild != NULL)
            rightCurve = postOrderTraversal(node->rightChild);
        // cout << "postOrderTraversal4" << endl;
        vector<vector<float>> meancurve=meanCurve(leftCurve, rightCurve);
        // cout << "postOrderTraversal5" << endl;
        double e=0.1;
        while(meancurve.size()>curveSize)
        {
            TWO_DIM::filter(meancurve,e);
            e*=2;
        }
        // cout << "postOrderTraversal6" << endl;
        return meancurve;
    }
}

bool Tree::isLeaf(Node* node)  
{
    // cout << "isLeaf" << endl;
    if(node->leftChild == NULL && node->rightChild == NULL){
        // cout << "YES" << endl;
        return true; 
    }
    else{
        // cout << "NO" << endl;
        return false;
    }
}

void Tree::placeChildren()
{
    while(1){
        cout << n << endl;
        floors.push_back(n);
        n = ceil(n/2.0);
        if(n==1){
            floors.push_back(1);
            break;
        }
    }
    structure = new Node**[floors.size()];
    Node* childx = NULL;
    Node* childy = NULL;
    for(int i=0;i<floors.size();i++){
        structure[i] = new Node*[floors[i]];
        if(i==0)
            for(int j=0;j<curves.size();j++)
                structure[i][j] = new Node(curves[j]);
        else if(i>0 && i<floors.size()-1)
            for(int j=0;j<floors[i]-1;j=j+2){
                if (j<floors[i]-1) childy = structure[i-1][j];
                else childx = NULL;
                if (j+1<floors[i]-1) childy = structure[i-1][j+1];
                else childy = NULL;
                structure[i][j] = new Node(childx, childy);
            }
        else if(i==floors.size()-1){
            structure[i][0] = new Node(childx, childy);
            root = structure[i][0];
        }
        cout<<i<<" "<<floors[i]<<endl;
    }
}

Tree::~Tree(){
    if(error==false){
        cout << "Deleting Tree" << endl;
        for(int i=0;i<floors.size();i++){
            if(i==0)
                for(int j=0;j<curves.size();j++)
                    delete structure[i][j];
            else if(i>0 && i<floors.size()-1)
                for(int j=0;j<floors[i]-1;j=j+2)
                    delete structure[i][j];
            else if(i==floors.size()-1)
                delete structure[i][0];
            delete[] structure[i];
        }
        delete[] structure;
    }
}

