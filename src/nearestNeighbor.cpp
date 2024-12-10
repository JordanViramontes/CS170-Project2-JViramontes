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

Validator::Validator(vector<unsigned int> &f, vector<shared_ptr<Id>> &realSet) {
    // Classifier
    classifier = Classifier(realSet, f);

    features = f;
    accuracy = validate(realSet);
}

double Validator::validate(vector<shared_ptr<Id>> &realSet) {
    vector<shared_ptr<KSet>> trainedVec = classifier.getTrainedSet();
    vector<int> wrongIterations;
    unsigned int total = 0;
    unsigned int correct = 0;

    string outputName = "output/tempoutput.txt";
    ofstream resetFile(outputName);
        if (resetFile.is_open()) {
            resetFile << "";
            resetFile.close();
        }

    // iterate over the classified vector trainedVec.size()
    for (unsigned int i = 0; i < trainedVec.size(); i++, total++) {        
        bool isTrue = false;
        // string outputStr = 
        // base cases: label/predictedLabel is undefined 
        if (trainedVec.at(i)->predictedLabel == -1 || realSet.at(i)->label == -1) continue;

        // if our predicted label is correct, increase correct counter
        if (trainedVec.at(i)->predictedLabel == realSet.at(i)->label) {
            isTrue = true;
            correct++;
        }
        else { wrongIterations.push_back(i); }

        // create output
        string str = to_string(isTrue) + ", ";
        for (unsigned int j = 0; j < features.size(); j++) {
            str += to_string(realSet.at(i)->features.at(features.at(j))) + " ";
        }
        str += "\n";
        
        // write to file
        ofstream file(outputName, std::ios_base::app);
        if (file.is_open()) {
            file << str;
            file.close();
        }
    }

    // cout << wrongIterations.at(wrongIterations.size()-1) << "}\n";
    return (correct * 1.0) / (total * 1.0);
}

// OUPUTKSET

OutputKSet::OutputKSet() {
    vector<unsigned int> features = {2, 7}; // good
    // vector<unsigned int> features = {1, 4}; // bad
    string str = "data/smallData.txt";
    parseDataset(str);


    Validator v = Validator(features, realSet);

    double accuracy = v.getAccuracy();
    cout << "Accuracy: " << accuracy << endl;
}

void OutputKSet::parseDataset(std::string dataFile) {
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
                if (min == -1 && label >= 0) min = number;
                if (max == -1) max = number;

                if (number < min && label >= 0) min = number;
                if (number > max) max = number;

                // if label is undefined, set label = number
                if (label < 0) { label = number; }
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
        // for (unsigned int i = 0; i < features.size(); i++) {
        //     cout << features.at(i) << ", ";
        // }
        // cout << ", L: " << label << endl;
        realSet.push_back( shared_ptr<Id>(new Id(label, features)) );
    } while (getline(file, str)  && !str.empty() );

    cout << "min: " << min << ", max: " << max << ", total set size: " << realSet.size() << endl;

    // // normalize data
    // for (unsigned int i = 0; i < realSet.size(); i++) {
    //     // go through id's features, if -1 then skip
    //     for (unsigned int j = 0; j < realSet.at(i)->features.size(); j++) {
    //         if (realSet.at(i)->features.at(j) == -1) continue;
    //         realSet.at(i)->features.at(j) = minmax(realSet.at(i)->features.at(j), min, max);
    //     }
    // }
}