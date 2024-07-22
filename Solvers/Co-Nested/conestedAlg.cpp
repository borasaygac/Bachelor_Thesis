#include "globals.hpp"
#include <unordered_map>

void conestedAlgorithm() {

    vector<int> varDegrees(numOfVars + 1, 0);

    for (int i = 1; i <= numOfVars; i++) {
        varDegrees[i] = vars[i].getTotalOccurances();
    }
    
}