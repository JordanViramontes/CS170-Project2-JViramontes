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
    value = calculateValue();
}

Node::Node(vector<unsigned int> v) {
    parent = nullptr;
    features = v;
    sort(v.begin(), v.end()); // sort features for later checks

    value = calculateValue();
}

double Node::calculateValue() {
    return rand() % 101;
}

void Node::prepareChildren(const unsigned int totalF, const bool isForward) {
    vector<int> childrenFeatures;

    if (isForward) {
        // add all possible features to children vector
        unsigned int j = 0;
        for (unsigned int i = 0; i < totalF; i++) {
            if ( (j < features.size()) && (i == features.at(j)) ) {
                j++;
                continue;
            }
            childrenFeatures.push_back(i);
        }

        // for each new child feature, make a new node to get all children
        for (unsigned int i = 0; i < childrenFeatures.size(); i++) {
            // create vector with current features + children feature
            vector<unsigned int> tempVec = features;
            tempVec.push_back(childrenFeatures.at(i));

            // create new node and add it to children
            shared_ptr<Node> tempNode = shared_ptr<Node>(new Node(tempVec));
            children.push_back(tempNode);
        }
    }
    else {
        //iterate over features and remove a features one at a time
        for (unsigned int i = 0; i < features.size(); i++) {
            // new vector with 1 feature removed
            vector<unsigned int> newFeatures = features;
            newFeatures.erase(newFeatures.begin() + i);

            // add to children
            shared_ptr<Node> tempChild = shared_ptr<Node>(new Node(newFeatures));
            children.push_back(tempChild);
        }
    }
}

// GRAPH

Graph::Graph() {
    initNode = nullptr;
    maxNode = nullptr;
    totalFeatures = 0;
    isForward = true;
}

Graph::Graph(int f, bool isF) {    
    totalFeatures = f;
    isForward = isF;
    
    // if we want forward, then init node has no features, if backward, it has all 4
    if (isForward) { initNode = shared_ptr<Node>(new Node()); }
    else {
        vector<unsigned int> temp;
        for (unsigned int i = 0; i < totalFeatures; i++) {
            temp.push_back(i);
        }

        initNode = shared_ptr<Node>(new Node(temp));
    }

    maxNode = initNode;
    allNodes.push_back(initNode);
}

void Graph::Search(shared_ptr<Node> n) {
    if (n->getFeatureCount() == totalFeatures && isForward) return; //Forward selection: we are at all features
    if (n->getFeatureCount() == 1 && !isForward) {
        //Backward elim: we have no features to remove
        shared_ptr<Node> noFeatures = shared_ptr<Node>(new Node());
        allNodes.push_back(noFeatures);
        cout << "Using no features: { } and \"random\" evaluation: " << noFeatures->getValue() << "%\n\n";

        // check node with no input
        if (maxNode->getValue() < noFeatures->getValue()) { maxNode = noFeatures; }
        
        return; 
    }

    n->prepareChildren(totalFeatures, isForward);

    // print features
    cout << "Possible new Feature Set(s):\n";
    for (unsigned int i = 0; i < n->getChildren().size(); i++) { 
        // print features:
        // cout << n->getChildren().at(i)->getValue() << ", ";
        cout << "Features: {";
        n->getChildren().at(i)->printFeatures();
        cout << "}: " << n->getChildren().at(i)->getValue() << "%\n";
    }
    cout << endl;

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

    // print maxnode
    cout << "Feature set: {";
    maxChild->printFeatures();
    cout << "} was best, accuracy: " << maxChild->getValue() << "%\n\n";

    Search(maxChild);
}

void Graph::Search() {
    if (isForward) cout << "Using no features: { } and \"random\" evaluation: " << maxNode->getValue() << "%\n\n";
    else {
        cout << "Using all features: {";
        maxNode->printFeatures();
        cout << "} and \"random\" evaluation: " << maxNode->getValue() << "%\n\n";
    }

    Search(initNode);

    cout << "Overall Best Feature set was: {";
    maxNode->printFeatures();
    cout << "}, accuracy: " << maxNode->getValue() << "%\n\n";
}