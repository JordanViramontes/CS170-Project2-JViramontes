#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h> 
#include <time.h>
#include <algorithm>

#include "classes.h"

using namespace std;




// NODE

Node::Node() {
    parent = nullptr;
    value = 0;
}

Node::Node(vector<unsigned int> v) {
    parent = nullptr;
    features = v;
    sort(v.begin(), v.end());

    cout << "TESTVEC: ";
    for (unsigned int i = 0; i < features.size(); i++) {
        cout << features.at(i) << ", ";
    }
    cout << endl;

    value = calculateValue();
}

double Node::calculateValue() {
    // srand (time(0));
    double temp = rand() % 101;
    cout << "RANDOM: " << temp << endl << endl;
    return temp;
}

void Node::prepareChildren(unsigned int totalF) {
    vector<int> childrenFeatures;

    // add all possible features to children vector
    unsigned int j = 0;
    for (unsigned int i = 0; i < totalF; i++) {
        if ( (j < features.size()) && (i == features.at(j)) ) {
            j++;
            continue;
        }
        childrenFeatures.push_back(i);
    }

    for (unsigned int i = 0; i < childrenFeatures.size(); i++) {
        cout << childrenFeatures.at(i) << ", ";
    }
    cout << endl;

    // for each new child feature, make a new node to get all children
    for (unsigned int i = 0; i < childrenFeatures.size(); i++) {
        // create vector with current features + children feature
        vector<unsigned int> tempVec = features;
        tempVec.push_back(childrenFeatures.at(i));

        // create new node and add it to children
        shared_ptr<Node> tempNode = shared_ptr<Node>(new Node(tempVec));
        children.push_back(tempNode);
    }

    return;
}

// GRAPH

Graph::Graph() {
    initNode = nullptr;
    maxNode = nullptr;
    totalFeatures = 0;
}

Graph::Graph(int f) {
    initNode = shared_ptr<Node>(new Node());
    maxNode = initNode;
    allNodes.push_back(initNode);
    
    totalFeatures = f;
}

void Graph::ForwardSelection(shared_ptr<Node> n) {
    if (n->getFeatureCount() == totalFeatures) return; //we've explored all features

    n->prepareChildren(totalFeatures);
    
    // get the best child possibility
    shared_ptr<Node> maxChild = nullptr;
    for (unsigned int i = 0; i < n->getChildren().size(); i++) {
        // add child to total vector
        allNodes.push_back(n->getChildren().at(i));

        // base case
        if (maxChild == nullptr) {
            maxChild = n->getChildren().at(i);
            continue;
        }
        
        // get new max
        if (maxChild->getValue() < n->getChildren().at(i)->getValue()) { maxChild = n->getChildren().at(i); }
    }

    // check to see if new child is better than current max
    if (maxChild->getValue() > maxNode->getValue()) { maxNode = maxChild; }

    ForwardSelection(maxChild);
}

void Graph::ForwardSelection() {
    ForwardSelection(initNode);
    cout << "MAX IS: " << maxNode->getValue() << endl;
}