#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include <vector>
#include <memory>
#include <string>

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

    KSet(vector<shared_ptr<Id>> tSet, shared_ptr<Id> tF) {
        trainSet = tSet;
        testFeature = tF;
    }
};

struct Dist {
    double distance = -1;
    double label = -1;
    int it = -1;

    Dist(double d, double l, int i) {
        distance = d;
        label = l;
        it = i;
    }
};

// for creating the set
inline bool operator <(const Dist &i, const Dist &j) { 
        double d1 = i.distance;
        double d2 = j.distance;
        return (d1 < d2); 
    }

// bool compareDist(const Dist &a, const Dist &b)
// {
//     return a.distance < b.distance;
// }

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
        vector<shared_ptr<Id>> realSet;
        Classifier classifier;
        double accuracy;
        std::string dataFile;

        double validate();
        void parseDataset();
        double minmax(const double x, const double min, const double max) {
            return (x - min) / (max - min);
        }


    public:
        Validator();
};


#endif 