#ifndef FEATURE_SELECT_H
#define FEATURE_SELECT_H

#include <iostream>
#include <memory>
#include <vector>

using std::vector, std::shared_ptr, std::cout;

class Node {
    private:
        shared_ptr<Node> parent;
        vector<shared_ptr<Node>> children;
        double value;
        vector<unsigned int> features;

        double calculateValue();
    public:
        Node();
        Node(vector<unsigned int>);
        void prepareChildren(const unsigned int totalF, const bool isForward);

        //get 
        const unsigned int getFeatureCount() { return features.size(); }
        const double getValue() { return value; }
        const vector<shared_ptr<Node>> getChildren() { return children; }
        const void printFeatures() {
            for (unsigned int i = 0; i < features.size()-1; i++) {
                cout << features.at(i) << ", ";
            }
            cout << features.at(features.size()-1);
        }
};

class Graph {
    private:
        shared_ptr<Node> initNode;
        shared_ptr<Node> maxNode;
        vector<shared_ptr<Node>> allNodes;
        unsigned int totalFeatures;
        bool isForward;
        void Search(shared_ptr<Node> n);
    public:
        Graph();
        Graph(int f, bool isForward);
        void Search();
};

#endif