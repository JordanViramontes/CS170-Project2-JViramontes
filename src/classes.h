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
        vector<int> features;

    public:
        Node();
        double calculateValue();
        void prepareChildren(unsigned int totalF);

        //get 
        unsigned int getFeatureCount() { return features.size(); }
};

class Graph {
    private:
        shared_ptr<Node> initNode;
        unsigned int totalFeatures;
        void ForwardSelection(shared_ptr<Node> n);
    public:
        Graph();
        Graph(int f);
        void ForwardSelection();
        
};

#endif