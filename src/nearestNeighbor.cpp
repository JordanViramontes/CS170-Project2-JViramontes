#include "headers/nearestNeighbor.h"

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

// Classifier

Classifier::Classifier(vector<shared_ptr<Id>> &realSet) {
    train(realSet);
    test();
}

double Classifier::distance(shared_ptr<Id> test, shared_ptr<Id> train) {
    vector<double> distanceVec;
    double finalDistance = 0;

    // get all (x1-x2) for all dimentions
    for (unsigned int i = 0; i < test->features.size(); i++) {
        distanceVec.push_back(test->features.at(i) - train->features.at(i));
    }

    // add all (x1-x2)^2
    for (unsigned int i = 0; i < distanceVec.size(); i++) {
        finalDistance += pow(distanceVec.at(i), 2);
    }

    // sqrt and return
    return sqrt(finalDistance);
}

void Classifier::train(vector<shared_ptr<Id>> &testSet) {

    for (unsigned int i = 0; i < testSet.size(); i++) {
        vector<shared_ptr<Id>> newSet = testSet;
        newSet.erase(newSet.begin() + i);

        shared_ptr<KSet> newKSet = shared_ptr<KSet>(new KSet(newSet, testSet.at(i)));

        for (unsigned int i = 0; i < newKSet->trainSet.size(); i++) {
            cout << newKSet->trainSet.at(i)->label << ", ";
        }
        // cout << "trained to: " << newKSet->testFeature->label << endl;

        fullTrainSet.push_back(newKSet);
    }

}

int Classifier::test() {
    for (unsigned int k = 0; k < fullTrainSet.size(); k++) {
        shared_ptr<KSet> temp = fullTrainSet.at(k);

        // cout << "CURRENT SET: " << endl;
        // for (unsigned int i = 0; i < temp->trainSet.size(); i++) {
        //         cout << temp->trainSet.at(i)->label << ", ";
        //     }
        //     cout << "trained to: " << temp->testFeature->label << endl;

        unsigned int nearestNeighbor = 0;
        double nearestNeighborDistance = -1;

        for (unsigned int i = 0; i < temp->trainSet.size(); i++) {
            double currentDist = distance(temp->testFeature, temp->trainSet.at(i));

            // if theres no nearest neighbor yet
            if (nearestNeighborDistance == -1) {
                nearestNeighbor = i;
                nearestNeighborDistance = currentDist;
            }

            // if theres a new closest distance
            if (currentDist < nearestNeighborDistance) {
                nearestNeighbor = i;
                nearestNeighborDistance = currentDist;
            }
            // cout << "\tlabel: " << temp->trainSet.at(i)->label << ", currentDist: " << currentDist
            //     << ", nearest: " << nearestNeighbor << ", " << nearestNeighborDistance << endl;
        }

        // set the Kset's label = nearestneighbor
        temp->predictedLabel = temp->trainSet.at(nearestNeighbor)->label;
        // cout << "predicted label: " << temp->predictedLabel << endl;
    }
    

    return 0;
}

// Validator

Validator::Validator() {
    dataFile = "datasets/small-test-dataset.txt";

    shared_ptr<Id> id0 = shared_ptr<Id>(new Id(1, {0.01, 0.02, 0.02}));
    shared_ptr<Id> id1 = shared_ptr<Id>(new Id(2, {0.01, 0.01, 0.03}));
    shared_ptr<Id> id2 = shared_ptr<Id>(new Id(1, {0.02, 0.03, 0.02}));
    shared_ptr<Id> id3 = shared_ptr<Id>(new Id(1, {0.03, 0.02, 0.02}));
    shared_ptr<Id> id4 = shared_ptr<Id>(new Id(2, {0.05, 0.01, 0.05}));
    
    realSet.push_back(id0);
    realSet.push_back(id1);
    realSet.push_back(id2);
    realSet.push_back(id3);
    realSet.push_back(id4);
    trainedSet = Classifier(realSet);

    accuracy = validate();
    cout << "total accuracy: " << accuracy << endl;
}

double Validator::validate() {
    vector<shared_ptr<KSet>> trainedVec = trainedSet.getTrainedSet();
    unsigned int total = 1;
    unsigned int correct = 0;

    for (unsigned int i = 0; i < trainedVec.size(); i++, total++) {
        // cout << total << ", predicted: " << trainedVec.at(i)->predictedLabel << ", real: " << realSet.at(i)->label << endl;
;
        if (trainedVec.at(i)->predictedLabel == realSet.at(i)->label) {
            correct++;
        }
    }

    return (correct * 1.0) / (total * 1.0);
}

void Validator::parseDataset() {
    ifstream file(dataFile);
    string str; 
    while (getline(file, str))
    {
        // Process str
    }
}