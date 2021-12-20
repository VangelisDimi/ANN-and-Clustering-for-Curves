#include "binary_tree.hpp"
#include "utils.hpp"
#include <algorithm>
#include <climits>
#include <math.h>     
#include "debug.hpp"

Tree::Tree(vector<cluster_Frechet::centroid_item> OGcurves)
{
    Tree::n = OGcurves.size();
    Tree::height = ceil(log2(n));
    Tree::root = NULL;
    Tree::mycounter = 0;
    if(OGcurves.size()>0)
        Tree::curveSize = OGcurves[0].p.size();
    else{
        error = true;
        return;
    }
    for(int i=0;i<n;i++)
        Tree::curves.push_back(OGcurves[i].p);
    debug("Constructing new Tree of height "<<height);
    int nChildren = n;
    while(1){
        debug(nChildren);
        floors.push_back(nChildren);
        nChildren = ceil(nChildren/2.0);
        if(nChildren==1){
            break;
        }
    }
    Tree::height = floors.size();
    root = createNode(height);
}

vector<vector<float>> Tree::postOrderTraversal(Node* node)
{
    debug("postOrderTraversal");
    vector<vector<float>> leftCurve;
    vector<vector<float>> rightCurve;
    if(isLeaf(node)){
        debug("isLeaf");
        return node->curve;
    }
    else
    {
        debug("isNotLeaf");
        leftCurve = postOrderTraversal(node->leftChild);
        debug("get left meanCurve");
        if (node->rightChild != NULL)
            rightCurve = postOrderTraversal(node->rightChild);
        debug("get right meanCurve");
        vector<vector<float>> meancurve=meanCurve(leftCurve, rightCurve);
        debug("filter meanCurve");
        double e=0.1;
        while(meancurve.size()>curveSize)
        {
            TWO_DIM::filter(meancurve,e);
            e*=2;
        }
        debug("return meanCurve");
        return meancurve;
    }
}

bool Tree::isLeaf(Node* node)  
{
    debug("isLeaf");
    if(node->leftChild == NULL && node->rightChild == NULL){
        debug("YES");
        return true; 
    }
    else{
        debug("NO");
        return false;
    }
}

Tree::~Tree(){
    deleteNode(root);
}

Node* Tree::createNode(int level)
{
    if(level>0){
        Node* leftChild = createNode(level-1);
        Node* rightChild = createNode(level-1);
        return new Node(leftChild, rightChild);
    }
    else{
        if (mycounter<curves.size()){
            debug("putting "<<mycounter<<" curve of "<<curves.size()<<" in tree of height "<<height);
            return new Node(curves[mycounter++], NULL, NULL);
        }
        else return NULL;
    }
}

void Tree::deleteNode(Node* node)
{
    if(node->leftChild!=NULL)
        deleteNode(node->leftChild);
    if(node->rightChild!=NULL)
        deleteNode(node->rightChild);
    delete node;
}


