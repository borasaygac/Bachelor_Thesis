#include "../../globals.hpp"
#include <algorithm>
#include <unordered_map>
#include <utility>

vector<pair<pair<int,int>, bool>> coNestedLessThanResArr;

vector<vector<int>> coNestedVariableOccs;

vector<int> varDegrees;

bool coNestedLessThanCompare(int a, int b) {
    if (coNestedVariableOccs[a][coNestedVariableOccs[a].size()-1] <= coNestedVariableOccs[b][0]){
        return true;
    } 
    return false;
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

void fillDegreesforVars() {
    for (int i = 1; i <= numOfVars; i++) {
        varDegrees[i] = vars[i].getTotalOccurances();
    }
}

void generateLessThanResults() {
    for (int i = 1; i <= numOfVars; i++) {
        for (int j = 1; j <= numOfVars; j++) {
            if (i != j) {
                bool result = coNestedLessThanCompare(i,j);
                coNestedLessThanResArr.push_back({{i,j},result});
            }
            
        }
    }
}

void conestedAlgorithm() {

    fillVarOccsArray();
    
    varDegrees.resize(numOfVars + 1);
    fillDegreesforVars();

    //int M = 0; // M denotes the number of simultaneously satisfiable clauses
    
    generateLessThanResults();
    



}