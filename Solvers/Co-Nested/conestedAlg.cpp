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

bool findPairAndGetValue(int a, int b) {
    for (const auto& item : coNestedLessThanResArr) {
        if (item.first.first == a && item.first.second == b) {
            return item.second; // Return the associated bool value
        }
    }
    // Optionally, handle the case where the pair is not found
    // For example, return false or throw an exception
    return false; // Example default behavior
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

bool coNestedPrecedesCompare(int a, int b) {
    if ((coNestedVariableOccs[a][0] <= coNestedVariableOccs[b][0]) & 
        (coNestedVariableOccs[b][varDegrees[b]-1] <= coNestedVariableOccs[a][varDegrees[a]-1])) {
            return true;
    }
    return false;
}

bool isDirectPredecessorInLess(int a, int b) {
    if (coNestedLessThanCompare(a, b)) {
        for (int z = 1; z <= numOfVars; z++) {
            if (z != a && z != b && coNestedLessThanCompare(a, z) && coNestedLessThanCompare(z, b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool isDirectPredecessorInPred(int a, int b) {
    if (coNestedPrecedesCompare(a, b)) {
        for (int z = 1; z <= numOfVars; z++) {
            if (z != a && z != b && coNestedPrecedesCompare(a, z) && coNestedPrecedesCompare(z, b)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

vector<int> findPrecMaximalElements(set<int> &variables) {
    vector<int> maximalElements;
    for (int x : variables) {
        bool isPrecMaximal = true;
        for (int y : variables) {
            if ((x != y) & coNestedPrecedesCompare(x, y)) {
                isPrecMaximal = false;
                break;
            }
        }
        if (isPrecMaximal) {
            maximalElements.push_back(x);
        }
    }
    return maximalElements;
}


void conestedAlgorithm() {

    fillVarOccsArray();
    
    varDegrees.resize(numOfVars + 1);
    fillDegreesforVars();

    //int M = 0; // M denotes the number of simultaneously satisfiable clauses
    
    generateLessThanResults();

    set<int> variables;
    for (int i = 1; i <= numOfVars; i++) {
        variables.insert(i);
    }

    vector<vector<int>> X;
    
    while (!variables.empty()) {
        vector<int> Xk = findPrecMaximalElements(variables);
        X.push_back(Xk);
        for (int x : Xk) {
            variables.erase(x);
        }
    }

    // Output the sets X^k
    for (size_t i = 0; i < X.size(); ++i) {
        printf("X^%i:", i);
        for (int x : X[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }


}