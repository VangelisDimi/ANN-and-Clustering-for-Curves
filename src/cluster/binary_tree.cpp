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
            // floors.push_back(1);
            break;
        }
    }
    Tree::height = floors.size();
    root = createNode(height);
    // placeChildren();
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

// void Tree::placeChildren()
// {
//     while(1){
//         debug(n);
//         floors.push_back(n);
//         n = ceil(n/2.0);
//         if(n==1){
//             floors.push_back(1);
//             break;
//         }
//     }
//     // root = new Node(new Node(), new Node());
//     // debug("root is "<<root<<" with children "<<root->leftChild<<" and "<<root->leftChild);
//     structure = new Node**[floors.size()];
//     Node* childx = NULL;
//     Node* childy = NULL;
//     for(int i=0;i<floors.size();i++){
//         structure[i] = new Node*[floors[i]];
//         if(i==0)
//             for(int j=0;j<curves.size();j++){
//                 structure[i][j] = new Node(curves[j]);
//                 debug("leaf " <<j<<" is "<<structure[i][j]);
//             }
//         else if(i>0 && i<floors.size()-1){
//             int counter=0;
//             for(int j=0;j<floors[i];j++){
//                 if (counter<floors[i]-1) childx = structure[i-1][counter++];
//                 else childx = NULL;
//                 if (counter<floors[i]-1) childy = structure[i-1][counter++];
//                 else childy = NULL;
//                 structure[i][j] = new Node(childx, childy);
//                 debug("node " <<j<<" is "<<structure[i][j]<<" with children "<<childx<<" and "<<childy);
//                 if(childx==NULL || childy==NULL){
//                     debug("breaking");
//                     break;
//                 }
//             }
//         }
//         else if(i==floors.size()-1){
//             structure[i][0] = new Node(childx, childy);
//             root = structure[i][0];
//             debug("root is "<<root);
//         }
//         debug(i<<" "<<floors[i]);
//     }
// }

Tree::~Tree(){
    // if(error==false){
    //     debug("Deleting Tree");
    //     for(int i=0;i<floors.size();i++){
    //         // if(i==0)
    //         //     for(int j=0;j<curves.size();j++)
    //         //         delete structure[i][j];
    //         // else 
    //         // if(i>0 && i<floors.size()-1)
    //         //     for(int j=0;j<floors[i]-1;j=j+2)
    //         //         delete structure[i][j];
    //         // else 
    //         if(i==floors.size()-1)
    //             delete structure[i][0];
    //         delete[] structure[i];
    //     }
    //     delete[] structure;
    // }
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


