#include <iostream>
#include "BALProblem.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc < 2)
    {
        cerr<<"Please Input Files Name!\n";
    }

    BALProblem problem(argv[1]);
    problem.solve();
    return 0;
}