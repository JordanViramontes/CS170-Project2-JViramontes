#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h> 
#include <time.h>
#include <algorithm>
#include <fstream>
#include <cmath>

#include "headers/featureSelect.h"
#include "headers/nearestNeighbor.h"

using namespace std;




// NODE

Node::Node() {
    parent = nullptr;
    value = -1;
}

Node::Node(vector<unsigned int> &v, vector<shared_ptr<Id>> &realSet) {
    parent = nullptr;
    features = v;

    Validator validator = Validator(v, realSet);
    value = validator.getAccuracy();
}

void Node::prepareChildren(const unsigned int totalF, const bool isForward, vector<shared_ptr<Id>> &realSet) {
    vector<int> childrenFeatures;

    if (isForward) {
        // add all possible features to children vector
        unsigned int j = 0;

        for (unsigned int i = 1; i < totalF; i++) {
            // if we already have this feature, continue
            if ( (j < features.size()) && (i == features.at(j)) ) {
                j++;
                continue;
            }

            // create new child and insert new feature in correct position
            vector<unsigned int> tempVec = features;
            if (features.size() == 0) { tempVec.push_back(i); }
            else { tempVec.insert(std::lower_bound(tempVec.begin(), tempVec.end(), i), i); }

            shared_ptr<Node> tempNode = shared_ptr<Node>(new Node(tempVec, realSet));
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
            shared_ptr<Node> tempChild = shared_ptr<Node>(new Node(newFeatures, realSet));
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

Graph::Graph(int f, bool isF, string dataFile) {    
    parseDataset(dataFile);

    // cout << "feature count: " << realSet.at(0)->features.size() << endl;

    totalFeatures = realSet.at(0)->features.size();
    isForward = isF;
    
    // // if we want forward, then init node has no features, if backward, it has all 4
    if (isForward) { initNode = shared_ptr<Node>(new Node()); }
    else {
        vector<unsigned int> temp;
        for (unsigned int i = 0; i < totalFeatures; i++) {
            temp.push_back(i);
        }
        sort(temp.begin(), temp.end());
        initNode = shared_ptr<Node>(new Node(temp, realSet));
    }

    maxNode = initNode;
    allNodes.push_back(initNode);
    Search();
}

void Graph::Search(shared_ptr<Node> n) {
    if (n->getFeatureCount() == totalFeatures-1 && isForward) return; //Forward selection: we are at all features
    if (n->getFeatureCount() == 1 && !isForward) { return; } // Backward selection
    // if (n->getFeatureCount() >= 1) return; // test base case
    
    n->prepareChildren(totalFeatures, isForward, realSet);

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

    // if theres a tie choose the simpler one
    if (maxChild->getValue() == maxNode->getValue()) {
        if (maxChild->getFeatureCount() < maxNode->getFeatureCount()) {
            maxNode = maxChild;
        }
    }

    // print maxnodes
    cout << "Feature set: {";
    maxChild->printFeatures();
    cout << "} was best, accuracy: " << maxChild->getValue() << "%\n";

    cout << "Feature set: {";
    maxNode->printFeatures();
    cout << "} is the current best, accuracy: " << maxNode->getValue() << "%\n\n";

    // if our new children give a worse accuracy, aka it will only get worse from here
    // if (maxChild->getValue() < maxNode->getValue()) { return; }

    // recursive
    Search(maxChild);
}

void Graph::Search() {
    if (isForward) {
        cout << "Searching with Forward Selection.\n\n";
    }
    else {
        cout << "Searching with Backward Elimination.\n\n";
        cout << "Using all features: {";
        maxNode->printFeatures();
        cout << "} and \"random\" evaluation: " << maxNode->getValue() << "%\n\n";
    }

    Search(initNode);

    cout << "Overall Best Feature set was: {";
    maxNode->printFeatures();
    cout << "}, accuracy: " << maxNode->getValue() << "%\n\n";
}

void Graph::parseDataset(std::string dataFile) {
    ifstream file(dataFile);
    string str; 
    int totalFeatures = 0;
    double min = -1;
    double max = -1;

    // parse through first string to get the total feature count
    if (getline(file, str) && !str.empty()) {
        for (unsigned int i = 2; i < str.size(); i++) {
            if(str.at(i) == ' ' || i == str.size()-1) {
                totalFeatures++;

                // get ready for next loop
                i = i+1;
            }
        }
    }
    else { return; }

    // parse input for ID values
    do
    {
        vector<double> features(totalFeatures, -1);
        string tempStr = "";
        int label = -1;
        unsigned int featureCnt = 1;

        // parse row
        for (unsigned int i = 2; i < str.size(); i++) {
            if (str.at(i) != ' ') {
                tempStr += str.at(i);
            }

            if(str.at(i) == ' ' || i == str.size()-1) {
                // parse final string
                double LHS = (double)(tempStr.at(0) - '0');
                double RHS = atoi( tempStr.substr(2, 7).c_str() ) * (pow(10, -7));
                int exp = atoi( tempStr.substr(11, 3).c_str() );
                if (tempStr.at(10) == '-') exp = exp * -1;
                double number = (LHS + RHS) * (pow(10, exp));

                // check min/max
                if (min == -1 && label > 0) min = number;
                if (max == -1) max = number;

                if (number < min && label > 0) min = number;
                if (number > max) max = number;

                // if label is undefined, set label = number
                if (label <= 0) { label = number; }
                else {
                    features.at(featureCnt) = number;
                    featureCnt++;
                }

                // reset tempstr and get ready for next loop
                tempStr = "";
                i = i+1;
            }
        }

        // push back real set with our newly made Id
        realSet.push_back( shared_ptr<Id>(new Id(label, features)) );
    } while (getline(file, str)  && !str.empty() );

    cout << "min: " << min << ", max: " << max << ", total set size: " << realSet.size() << endl;

    // normalize data
    for (unsigned int i = 0; i < realSet.size(); i++) {
        // go through id's features, if -1 then skip
        for (unsigned int j = 0; j < realSet.at(i)->features.size(); j++) {
            if (realSet.at(i)->features.at(j) == -1) continue;
            realSet.at(i)->features.at(j) = minmax(realSet.at(i)->features.at(j), min, max);
        }
    }
}