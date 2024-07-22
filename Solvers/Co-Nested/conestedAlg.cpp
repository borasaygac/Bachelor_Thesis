#include "../../globals.hpp"
#include <algorithm>
#include <unordered_map>

vector<vector<bool>> coNestedLessThan(numOfVars + 1, vector<bool>(numOfVars + 1, false));

vector<vector<int>> coNestedVariableOccs;

bool coNestedLessThanCompare(int a, int b) {
    
}

void fillVarOccsArray() {
    coNestedVariableOccs.resize(numOfVars + 1);

    for (int i = 1; i <= numOfVars; i++) {
        // Add both occurances to a vector for future reference
        

        set<int> posOccurrences = vars[i].getPositiveOccurrances();
        set<int> negOccurrences = vars[i].getNegativeOccurrances();

        vector<int> mergedOccs(posOccurrences.size() + negOccurrences.size());

        // Merge the occurrences and sort
        std::merge(posOccurrences.begin(), posOccurrences.end(), negOccurrences.begin(), negOccurrences.end(), mergedOccs.begin());

        // Assign to the main vector
        coNestedVariableOccs[i].resize(mergedOccs.size());
        coNestedVariableOccs[i] = mergedOccs;

        
    }
}

void conestedAlgorithm() {

    fillVarOccsArray();
    
    int M = 0; // M denotes the number of simultaneously satisfiable clauses
    vector<int> varDegrees(numOfVars + 1, 0);

    for (int i = 1; i <= numOfVars; i++) {
        varDegrees[i] = vars[i].getTotalOccurances();
    }



}