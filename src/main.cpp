/*
DEVELOPED BY JORDAN VIRAMONTES

Disclaimer:
ALL code in this project was written by me and is entirely original given help from the following sources:

overloading operator for struct in order to make the set of distances work:
https://stackoverflow.com/questions/5816658/how-to-have-a-set-of-structs-in-c

recording time:
https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
*/

#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <string>
#include <fstream>

#include "headers/featureSelect.h"
#include "headers/nearestNeighbor.h"

using namespace std;

int main(int argc, char *argv[]) {
    bool cont = true;
    if (argc != 2) {
        cout << "Command arguments invalid, Exiting...\n";
        cont = false;
        return 0;
    }
    string filePath(argv[1]);
    cout << "using dataset: " << filePath << "\n\n";
    
    ifstream file(filePath); 
    if (!file) { 
        cout << "Provided file does not exist, Exiting...\n";
        cont = false;
        return 0;
    }

    while(cont) {
        cout << "Type in the features you'd like to test seperated by a 0, or q to quit:\n\t>";
        string str;
        getline(cin, str);

        // Parse input
        vector<unsigned int> features;
        string tempStr = "";
        for (unsigned int i = 0; i < str.size(); i++) {
            // check for q
            if (str.at(i) == 'q') {
                cout << "Exiting...\n";
                cont = false;
                break;
            }

            // build tempstr
            if (str.at(i) != ' ') {
                tempStr += str.at(i);
            }

            // if we have a space
            if(str.at(i) == ' ' || i == str.size()-1) {
                features.push_back( (unsigned int)(atoi( tempStr.substr(0, tempStr.size()).c_str() )) );
                tempStr = "";
            }
        }
        
        // validator
        if (cont) {
            cout << "\n\n";
            Validator v(features, filePath);
        }
    }
    return 0;
}