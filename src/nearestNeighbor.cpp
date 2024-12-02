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

// #include <bits/stdc++.h>

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Classifier

Classifier::Classifier(vector<shared_ptr<Id>> &realSet, vector<unsigned int> &features) {
    // Train
    cout << "Training dataset...\t";
    auto t1 = high_resolution_clock::now();
    train(realSet);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    cout << "Time: " << ms_double.count() << " ms\n";

    // Test
    cout << "Testing dataset...\t";
    t1 = high_resolution_clock::now();
    test(features);
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    cout << "Time: " << ms_double.count() << " ms\n\n";
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

        if (a == -1 && b == -1) {
            continue;
        }
        else if (a == -1) {
            distanceVec.push_back(b);
        }
        else if (b == -1) {
            distanceVec.push_back(a);
        }
        else {
            distanceVec.push_back(a - b);
        }
    }

    // add all (x1-x2)^2
    for (unsigned int i = 0; i < distanceVec.size(); i++) {
        finalDistance += pow(distanceVec.at(i), 2);
    }

    // sqrt and return
    if (distanceVec.size() <= 0) return -1;
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
        vector<Dist> distances;

        // loop over train set to fill the distance vector
        for (unsigned int i = 0; i < currentKSet->trainSet.size(); i++) {
            double currentDist = distance(currentKSet->testFeature, currentKSet->trainSet.at(i), featuresToTest);

            // if distance returns -1, aka all values were -1
            if (currentDist < 0) { continue; }

            distances.push_back(Dist(currentDist, currentKSet->trainSet.at(i)->label));
        }

        // set will sort the distance vector into ascending order in O(logn)
        set<Dist> distSet(distances.begin(), distances.end());

        // set the Kset's label = nearestneighbor
        currentKSet->predictedLabel = distSet.begin()->label;
        currentKSet->nearestDistance = distSet.begin()->distance;
    }
}

// Validator

Validator::Validator(vector<unsigned int> &features, string filePath) {
    dataFile = filePath;
    
    // Parse
    cout << "Parsing \"" << dataFile << "\"...\n\t";
    auto t1 = high_resolution_clock::now();
    parseDataset();
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    cout << "Time: " << ms_double.count() << " ms\n\n";

    // Classifier
    classifier = Classifier(realSet, features);

    // Validate
    cout << "Validating dataset...\n\t";
    t1 = high_resolution_clock::now();
    accuracy = validate();
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    cout << "Time: " << ms_double.count() << " ms\n\n";

    cout << "total accuracy: " << accuracy << endl;
}

double Validator::validate() {
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

    cout << "The iterations that predicted a wrong label are (Total incorrect: " 
         << total-correct << "/" << total << "): \n\t\t{";
    int cnt = 0;
    for (unsigned int i = 0; i < wrongIterations.size()-1; i++, cnt++) {
        cout << wrongIterations.at(i) << ", ";
        if (cnt >= 20) {
            cnt = 0;
            cout << "\n\t\t ";
        }
    }
    cout << wrongIterations.at(wrongIterations.size()-1) << "}\n";
    return (correct * 1.0) / (total * 1.0);
}

void Validator::parseDataset() {
    ifstream file(dataFile);
    string str; 
    int totalFeatures = 55;
    double min = -1;
    double max = -1;

    // parse input
    while (getline(file, str))
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
    }

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
