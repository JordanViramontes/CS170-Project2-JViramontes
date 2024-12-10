/*
DEVELOPED BY JORDAN VIRAMONTES

Disclaimer:
ALL code in this project was written by me and is entirely original given help from the following sources:

overloading operator for struct in order to make the set of distances work:
https://stackoverflow.com/questions/5816658/how-to-have-a-set-of-structs-in-c

recording time:
https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c

RESULTS:
Group: Jordan Viramontes - jvira007 - Section 22
- Small Dataset Results:
    Forward: Feature Subset: {3, 5}, Acc: 0.910891
    Backward: Feature Subset: {2, 4, 5, 7, 10} Acc: 0.831683
- Large Dataset Results:
    Forward: Feature Subset: {1, 27}, Acc: 0.955
    Backward: Feature Subset: {23,36,12}, Acc: 0.96
- Titanic Dataset Results:
    Forward:
    Backward: Feature Subset: {3, 4, 5}, Acc: 0.969188
*/

#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <string>
#include <fstream>
#include <chrono>

#include "headers/featureSelect.h"
#include "headers/nearestNeighbor.h"

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main(int argc, char *argv[]) {
    //note: use {3, 5} for good features and {3, 5} for good
    //      use {2, 7} or bad features

    //debug
    if (argc == 3) {
        string strDebug = argv[2];
        if (strDebug == "-1") {
            OutputKSet k;
            return 0;
        }
    }
    

    // normal
    if (argc != 2) {
        cout << "Command arguments invalid, Exiting...\n";
        return 0;
    }
    string filePath(argv[1]);
    cout << "using dataset: " << filePath << "\n\n";
    
    ifstream file(filePath); 
    if (!file) { 
        cout << "Provided file does not exist, Exiting...\n";
        return 0;
    }
    string data = argv[1];

    cout << "Do you want to use Forward, or Backward Selection? (Type 1 for forward and 2 for backward)\n\t> ";
    string str;
    getline(cin, str);

    // string str = "1";

    auto t1 = high_resolution_clock::now();
    if (str == "1") {
        Graph(0, 1, data);
    }
    else Graph(0, 0, data);

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "time: " << ms_double.count() << "ms\n";

    return 0;
}