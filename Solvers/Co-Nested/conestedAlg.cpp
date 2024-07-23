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


bool coNestedLessThanWithCurlyLineBelow(int x, int y, const vector<vector<int>>& levels) {
    int xlevel = -1; int ylevel = -1;
    for (size_t k = 0; k < levels.size(); ++k) {
        if (find(levels[k].begin(), levels[k].end(), x) != levels[k].end()) {
            xlevel = k;
        }
        if (find(levels[k].begin(), levels[k].end(), y) != levels[k].end()) {
            ylevel = k;
        }
    }

    if (xlevel != ylevel || xlevel == -1 || ylevel == -1) {
        return false;
    }

    if (!coNestedLessThanCompare(x, y)) {
        return false;
    }

    if (xlevel == 0) {
        return true;
    }

    // check if there is a variable z in level xlevel-1 such that x(degree(x) <= z(i) <= y(1))
    const vector<int>& sublevel = levels[xlevel-1];
    for (int z: sublevel){
        for (int i = 0; i < varDegrees[z]; ++i) {
            if (coNestedVariableOccs[x][varDegrees[x]-1] <= coNestedVariableOccs[z][i] 
            && (coNestedVariableOccs[z][i] <= coNestedVariableOccs[y][0])) {
                return false;
            }
        }
    }

    return true;
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

    // Example: Check the relation for some pairs
    printf("Relation results: \n");
    for (size_t i = 0; i < numOfVars; ++i) {
        for (size_t j = 0; j < numOfVars; ++j) {
            if (i != j) {
                printf("%i lesswithcurlylinebelow %i: %i\n",i+1,j+1, coNestedLessThanWithCurlyLineBelow(i+1, j+1, X));
            }
        }
    }

    int x_min = 0;
    int x_minDegree = 0; 
    for (size_t i = 0; i < coNestedCNF[1].size(); i++){
        if (varDegrees[coNestedCNF[1][i]] > x_minDegree) {
            x_min = coNestedCNF[1][i];
            x_minDegree = varDegrees[coNestedCNF[1][i]];
        }
    }
    printf("x_min: %i\n", x_min);
    printf("x_minDegree: %i\n", x_minDegree);

    int x_max = 0;
    int x_maxDegree = 0;
    for (size_t i = 0; i < coNestedCNF[coNestedCNF.size()-1].size(); i++){
        if (varDegrees[coNestedCNF[coNestedCNF.size()-1][i]] > x_maxDegree) {
            x_max = coNestedCNF[coNestedCNF.size()-1][i];
            x_maxDegree = varDegrees[coNestedCNF[coNestedCNF.size()-1][i]];
        }
    }
    printf("x_max: %i\n", x_max);
    printf("x_maxDegree: %i\n", x_maxDegree);

    printf("conested cnf: \n");
    for (size_t i = 0; i < coNestedCNF.size(); i++) {
        for (size_t j = 0; j < coNestedCNF[i].size(); j++) {
            printf("%i ", coNestedCNF[i][j]);
        }
        printf("\n");
    }

}