#include "headers/nearestNeighbor.h"

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>
#include <set>

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

int Classifier::test(vector<unsigned int> &featuresToTest) {
    for (unsigned int k = 0; k < allKSets.size(); k++) {
        shared_ptr<KSet> temp = allKSets.at(k);
        vector<Dist> distances;
        int finalLabel = -1;

        // cout << "loop: " << k << endl; 
        for (unsigned int i = 0; i < temp->trainSet.size(); i++) {
            double currentDist = distance(temp->testFeature, temp->trainSet.at(i), featuresToTest);

            // if distance returns -1, aka all values were -1
            if (currentDist < 0) { continue; }
            
            distances.push_back(Dist(currentDist, temp->trainSet.at(i)->label, i));
        }
        // use set for sorting distances
        set<Dist> distSet(distances.begin(), distances.end());
        

        // if the most recent 2 labels are equal, check the third 
        // for (set<Dist>::iterator i = distSet.begin(); i != distSet.end(); i++){
        //     cout << "CHECK : " << i->distance << ", l: " << i->label << ", i: " << i->it << endl;
        // }

        if (distSet.begin()->distance == next(distSet.begin())->distance &&
            distSet.begin()->label != next(distSet.begin())->label)
        {
            finalLabel = next(next(distSet.begin()))->label;
            // cout << "EQUAL, THUS: " << finalLabel << ", d: " << next(next(distances.begin()))->distance << endl;
        }
        else finalLabel = distSet.begin()->label;

        // set the Kset's label = nearestneighbor
        temp->predictedLabel = finalLabel;
        // cout << "predicted label: " << temp->predictedLabel << 
        //      ", at: " << nearestNeighbor << ", with: " << nearestNeighborDistance << endl;
    }
    

    return 0;
}

// Validator

Validator::Validator() {
    dataFile = "datasets/small-test-dataset.txt";
    parseDataset();

    // for (unsigned int i = 0; i < realSet.size(); i++) {
    //     cout << i << ". " << realSet.at(i)->label << ",\t";

    //     for (unsigned int j = 1; j < realSet.at(i)->features.size(); j++) {
    //         cout << realSet.at(i)->features.at(j) << ", ";
    //     }
    //     cout << endl;
    // }

    vector<unsigned int> featuresToTest = {3, 5, 7};
    classifier = Classifier(realSet, featuresToTest);
    accuracy = validate();
    cout << "total accuracy: " << accuracy << endl;
}

double Validator::validate() {
    vector<shared_ptr<KSet>> trainedVec = classifier.getTrainedSet();
    unsigned int total = 0;
    unsigned int correct = 0;

    for (unsigned int i = 0; i < trainedVec.size(); i++, total++) {
        // cout << total << ", predicted: " << trainedVec.at(i)->predictedLabel << ", real: " << realSet.at(i)->label << endl;
        if (realSet.at(i)->label == -1) continue;
        if (trainedVec.at(i)->predictedLabel == -1) continue;

        cout << "check: tr:" << trainedVec.at(i)->predictedLabel << ", ts: " << realSet.at(i)->label;

        if (trainedVec.at(i)->predictedLabel == realSet.at(i)->label) {
            correct++;
            cout << endl;
        }
        else cout << "WRONG!!, at loop: " << i << endl;
    }

    return (correct * 1.0) / (total * 1.0);
}

void Validator::parseDataset() {
    ifstream file(dataFile);
    string str; 
    int totalFeatures = 11;
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

                // std::cout << fixed;  
                // cout << tempStr << " -> " << number
                //     << ", feature: " <<  feature << endl;

                // if this is the first feature, set label = feature option, else add to features
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

        // for (unsigned int i = 0; i < features.size(); i++) {
        //     cout << i << ". " << features.at(i) << endl;
        // }

        // push back real set with our newly made Id
        realSet.push_back( shared_ptr<Id>(new Id(label, features)) );
        // cout << "label: " << realSet.at(realSet.size()-1)->label << ", ";
        // for (unsigned int i = 0; i < realSet.at(realSet.size()-1)->features.size(); i++) {
        //     cout << realSet.at(realSet.size()-1)->features.at(i) << ", ";
        // }
        // cout << endl;
    }

    cout << "min: " << min << ", max: " << max << ", total set size: " << realSet.size() << endl;

    // normalize data
    // for (unsigned int i = 0; i < realSet.size(); i++) {
    //     // go through id's features, if -1 then skip
    //     for (unsigned int j = 0; j < realSet.at(i)->features.size(); j++) {
    //         if (realSet.at(i)->features.at(j) == -1) continue;
    //         realSet.at(i)->features.at(j) = minmax(realSet.at(i)->features.at(j), min, max);
    //     }
    // }
}   

double Validator::minmax(const double x, const double min, const double max) {
    return (x - min) / (max - min);
}