/*
DEVELOPED BY JORDAN VIRAMONTES

Disclaimer:
ALL code in this project was written by me and is entirely original
*/

#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <string>

#include "classes.h"

using namespace std;

int main() {
    srand(18);
    bool isTrue = true;

    while(isTrue) {
        cout << "Welcome to 862284516 Forward Selection / Backward Elimination"
            << "\n Type \"1\" for Forward Selection, \"2\" for Backward Elimination, or \"q\" to quit:"
            << "\n\t> ";

        string getInput;
        getline(cin, getInput);
        cout << endl;

        if (getInput.size() == 0) getInput = 'q';

        switch (getInput.at(0)) {
            case ('1'): {
                Graph g = Graph(4, true);
                g.Search();
                break;
            }
            case ('2'): {
                Graph g = Graph(4, false);
                g.Search();
                break;
            }
            case ('q'): {
                cout << "Quitting...\n\n";
                isTrue = false;
                break;
            }
            default: {
                cout << "Invalid input, try again\n" << endl;
                break;
            }
        }
    }

    return 0;
}