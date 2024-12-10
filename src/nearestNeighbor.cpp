#include "headers/nearestNeighbor.h"

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <set>
#include <chrono>
#include <algorithm>

// #include <bits/stdc++.h>

using namespace std;

// Classifier

Classifier::Classifier(vector<shared_ptr<Id>> &realSet, vector<unsigned int> &features) {
    train(realSet);
    test(features);
}

double Classifier::distance(shared_ptr<Id> test, shared_ptr<Id> train, vector<unsigned int> featuresToTest) {
    vector<double> distanceVec;
    double finalDistance = 0;

    // get all (x1-x2) for all dimentions
    for (unsigned int i = 0; i < featuresToTest.size(); i++) {
        // continue if we have an invalid feature
        if (featuresToTest.at(i) > train->features.size()) continue;

        double a = test->features.at(featuresToTest.at(i));
        double b = train->features.at(featuresToTest.at(i));

        if (a == -1 && b == -1) { continue; }
        else if (a == -1) { finalDistance += b*b; }
        else if (b == -1) { finalDistance += a*a; }
        else { finalDistance += (a-b)*(a-b); }
    }

    // add all (x1-x2)^2
    for (unsigned int i = 0; i < distanceVec.size(); i++) {
        finalDistance += pow(distanceVec.at(i), 2);
    }

    // sqrt and return
    return sqrt(finalDistance);
}

void Classifier::train(vector<shared_ptr<Id>> &testSet) {
    // iterate over set of Ids, goal is to divide full set into a vector of KSets
    for (unsigned int i = 0; i < testSet.size(); i++) {
        // get new set and 
        vector<shared_ptr<Id>> newSet = testSet;
        newSet.erase(newSet.begin() + i);

        allKSets.push_back(shared_ptr<KSet>(new KSet(newSet, testSet.at(i))));
    }
}

void Classifier::test(vector<unsigned int> &featuresToTest) {
    // iterate over all ksets
    for (unsigned int k = 0; k < allKSets.size(); k++) {
        shared_ptr<KSet> currentKSet = allKSets.at(k);
        double smallestDistance = -1;
        int label = -1;

        // loop over train set to fill the distance vector
        for (unsigned int i = 0; i < currentKSet->trainSet.size(); i++) {
            double currentDist = distance(currentKSet->testFeature, currentKSet->trainSet.at(i), featuresToTest);

            // if distance returns -1, aka all values were -1
            if (currentDist < 0) { continue; }

            // first loop
            if (label < 0 || (label > 0 && currentDist < smallestDistance)) {
                label = currentKSet->trainSet.at(i)->label;
                smallestDistance = currentDist;
            }            
        }

        // set kset elements
        currentKSet->predictedLabel = label;
        currentKSet->nearestDistance = smallestDistance;
    }
}

// Validator

Validator::Validator(vector<unsigned int> &features, vector<shared_ptr<Id>> &realSet) {
    // Classifier
    classifier = Classifier(realSet, features);

    accuracy = validate(realSet);
}

double Validator::validate(vector<shared_ptr<Id>> &realSet) {
    vector<shared_ptr<KSet>> trainedVec = classifier.getTrainedSet();
    vector<int> wrongIterations;
    unsigned int total = 0;
    unsigned int correct = 0;

    // iterate over the classified vector
    for (unsigned int i = 0; i < trainedVec.size(); i++, total++) {
        // base cases: label/predictedLabel is undefined 
        if (trainedVec.at(i)->predictedLabel == -1 || realSet.at(i)->label == -1) continue;

        // if our predicted label is correct, increase correct counter
        if (trainedVec.at(i)->predictedLabel == realSet.at(i)->label) {
            correct++;
        }
        else { wrongIterations.push_back(i); }
    }

    // cout << wrongIterations.at(wrongIterations.size()-1) << "}\n";
    return (correct * 1.0) / (total * 1.0);
}
