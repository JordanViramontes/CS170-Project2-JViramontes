#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h> 
#include <time.h>

#include "classes.h"

using namespace std;

// NODE

Node::Node() {
    parent = nullptr;
    value = 0;
}

double Node::calculateValue() {
    // srand (time(0));
    srand(0);
    double temp = rand() % 101;
    cout << "RANDOM: " << temp << endl;
    return temp;
}

void Node::prepareChildren(unsigned int totalF) {
    vector<int> childrenFeatures;

    // add all possible features to children vector
    for (unsigned int i = 0, j = 0; i < totalF; i++) {
        if ( (j < features.size()) && (i = features.at(j)) ) {
            j++;
            continue;
        }
        childrenFeatures.push_back(i);
    }
    
    // for each new child feature, make a new node and perform greedy


    return;
}

// GRAPH

Graph::Graph() {
    initNode = nullptr;
    totalFeatures = 0;
}

Graph::Graph(int f) {
    initNode = shared_ptr<Node>(new Node());
    totalFeatures = f;
}

void Graph::ForwardSelection(shared_ptr<Node> n) {
    if (n->getFeatureCount() == totalFeatures) return; //we're at the final feature

    n->prepareChildren(totalFeatures);
}

void Graph::ForwardSelection() {
    ForwardSelection(initNode);
}