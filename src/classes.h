#ifndef CLASSES_H
#define CLASSES_H

#include <memory>
#include <vector>

using std::vector, std::shared_ptr;

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
        void prepareChildren(unsigned int totalF);

        //get 
        const unsigned int getFeatureCount() { return features.size(); }
        const double getValue() { return value; }
        const vector<shared_ptr<Node>> getChildren() { return children; }
};

class Graph {
    private:
        shared_ptr<Node> initNode;
        shared_ptr<Node> maxNode;
        vector<shared_ptr<Node>> allNodes;
        unsigned int totalFeatures;
        void ForwardSelection(shared_ptr<Node> n);
    public:
        Graph();
        Graph(int f);
        void ForwardSelection();
        
};

#endif