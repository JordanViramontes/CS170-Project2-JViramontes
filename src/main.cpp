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
#include <chrono>

#include "headers/featureSelect.h"
#include "headers/nearestNeighbor.h"

using namespace std;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main(int argc, char *argv[]) {
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

    auto t1 = high_resolution_clock::now();
    Graph(0, 1, data);
    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << "time: " << ms_double.count() << "ms\n";

    
    
    
    return 0;
}