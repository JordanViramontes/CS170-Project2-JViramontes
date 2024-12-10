#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include <vector>
#include <memory>
#include <string>

using std::shared_ptr, std::vector;

// each Id has a label and features
struct Id {
    int label;
    vector<double> features;

    Id(int l, vector<double> &f) {
        label = l;
        features = f;
    }
};

// represents each "cut" of the overall data set to be tested
struct KSet {
    vector<shared_ptr<Id>> trainSet;
    shared_ptr<Id> testFeature;
    int predictedLabel = -1;
    double nearestDistance = -1;

    KSet(vector<shared_ptr<Id>> tSet, shared_ptr<Id> tF) {
        trainSet = tSet;
        testFeature = tF;
    }
};

// used for linking a distance with label for classifier
struct Dist {
    double distance = -1;
    double label = -1;

    Dist(double d, double l) {
        distance = d;
        label = l;
    }
};

// for creating set of distances
inline bool operator <(const Dist &i, const Dist &j) { 
    double d1 = i.distance;
    double d2 = j.distance;
    return (d1 < d2); 
}

class Classifier {
    private:
        vector<shared_ptr<KSet>> allKSets;

        double distance(shared_ptr<Id> test, shared_ptr<Id> train, vector<unsigned int> featuresToTest);
        void train(vector<shared_ptr<Id>> &realSet);
        void test(vector<unsigned int> &featuresToTest);

    public:
        Classifier() {}
        Classifier(vector<shared_ptr<Id>> &realSet, vector<unsigned int> &features);
        vector<shared_ptr<KSet>> &getTrainedSet() { return allKSets; }

        
};

class Validator {
    private:
        vector<unsigned int> features;
        Classifier classifier;
        double accuracy;

        double validate(vector<shared_ptr<Id>> &realSet);
        double minmax(const double x, const double min, const double max) {
            return (x - min) / (max - min);
        }

    public:
        Validator(vector<unsigned int> &features, vector<shared_ptr<Id>> &realSet);
        double getAccuracy() { return accuracy; }
};

class OutputKSet {
    private:
        vector<shared_ptr<Id>> realSet;

        void parseDataset(std::string str);
        double minmax(const double x, const double min, const double max) {
            // return (x - min) / (max - min);
            return x;
        }

    public:
        OutputKSet();
};


#endif 