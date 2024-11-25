#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include <vector>
#include <memory>

using std::shared_ptr, std::vector;

struct Id {
    int label;
    vector<double> features;

    Id(int l, vector<double> f) {
        label = l;
        features = f;
    }
};

struct KSet {
    vector<shared_ptr<Id>> trainSet;
    shared_ptr<Id> testFeature;
    int predictedLabel = -1;

    KSet(vector<shared_ptr<Id>> &tSet, shared_ptr<Id> &tF) {
        trainSet = tSet;
        testFeature = tF;
    }
};

class Classifier {
    private:
        vector<shared_ptr<KSet>> fullTrainSet;

        double distance(shared_ptr<Id> test, shared_ptr<Id> train);
        void train(vector<shared_ptr<Id>> &realSet);
        int test();

    public:
        Classifier() {}
        Classifier(vector<shared_ptr<Id>> &realSet);
        vector<shared_ptr<KSet>> &getTrainedSet() { return fullTrainSet; }

        
};

class Validator {
    private:
        vector<shared_ptr<Id>> realSet;
        Classifier trainedSet;
        double accuracy;
        string dataFile;

        double validate();
        void parseDataset();

    public:
        Validator();
};


#endif 