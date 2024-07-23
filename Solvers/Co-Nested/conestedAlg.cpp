#include "../../globals.hpp"
#include <algorithm>
#include <unordered_map>
#include <utility>

vector<pair<pair<int,int>, bool>> coNestedLessThanResArr;

vector<vector<int>> coNestedVariableOccs;

vector<int> varDegrees;

unordered_map<int, set<int>> triangleSets;

unordered_map<tuple<int,int,bool,bool>, int> memoG;
unordered_map<tuple<int,bool,bool>, int> memoF;

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

void defineTriangleSets() {
    for (int i = 1; i <= numOfVars; i++) {
        int varDegree = varDegrees[i];
        for (int j = 0; j < varDegree - 1; j++) {
            int startClause = coNestedVariableOccs[i][j];
            int endClause = coNestedVariableOccs[i][j+1];
            printf("for variable %i with j %i, startClause: %i, endClause: %i\n", i, j, startClause, endClause);
            printf("startClause: %i, endClause: %i\n", startClause, endClause);
            set<int> triangleSet;

            for (int clauseIDx = startClause; clauseIDx <= endClause; clauseIDx++) {
                for (int lit : coNestedCNF[clauseIDx]) {
                    int absLit = abs(lit);
                    triangleSet.insert(absLit);
                }
            }

            triangleSets[i * 100 + j] = triangleSet; 
        }
    }
}

int findXMin (const vector<vector<int>>& X) {
    int xMin = X[0][0];
    for (int i : X[0]) {
        if (varDegrees[xMin] < varDegrees[i] && coNestedVariableOccs[i][0] == 1) {
            xMin = i;
        }
    }
    return xMin;
}

int findXMax (const vector<vector<int>>& X) {
    int xMax = X[0][0];
    int lowestOcc = coNestedVariableOccs[xMax][0];
    int highestOcc = coNestedVariableOccs[xMax][varDegrees[xMax]-1];
    for (int i : X[0]) {
        if (coNestedVariableOccs[xMax][varDegrees[xMax]] < coNestedVariableOccs[i][varDegrees[i]] &&
            coNestedVariableOccs[i][0] < lowestOcc) {
            xMax = i;
            lowestOcc = coNestedVariableOccs[i][0];
            highestOcc = coNestedVariableOccs[i][varDegrees[i]-1];
        }
    }
    return xMax;
}

// Function to compute f^\epsilon(x, alpha, beta)_ii+1
int computeFiiPlus1(int x, bool epsilon, bool alpha, bool beta, int i) {
    int thetaEpsilon = computeThetaEpsilon(x, epsilon, alpha, beta, i);

    auto [xMin, xMax] = findMinMaxinTriangle(x,i);
    if (xMin == INT_MAX || xMax == INT_MIN){
        return thetaEpsilon;
    }

    // Four cases 
    int maxValue = INT_MIN;
    for (bool alphaPrime : {true, false}) {
        for (bool alphaDoublePrime : {true, false}) {
            for (bool betaPrime : {true, false}) {
                for (bool betaDoublePrime : {true, false}) {
                int current = thetaEpsilon;

                    if (coNestedVariableOccs[x][i] == coNestedVariableOccs[xMin][0] && 
                        coNestedVariableOccs[xMax].back() == coNestedVariableOccs[x][i + 1]) {
                        current +=  g(xMin, xMax, alphaDoublePrime, betaDoublePrime) - (alphaDoublePrime && betaDoublePrime ? 1 : 0);
                        maxValue = max(maxValue,current);
                    } else if (coNestedVariableOccs[x][i] < coNestedVariableOccs[xMin][0] && 
                               coNestedVariableOccs[xMax].back() == coNestedVariableOccs[x][i + 1]){
                        current += g(xMin, xMax, alphaDoublePrime, betaDoublePrime) - (betaDoublePrime ? 1 : 0);
                        maxValue = max(maxValue, current);
                    } else if (coNestedVariableOccs[x][i] == coNestedVariableOccs[xMin][0] &&
                               coNestedVariableOccs[xMax].back() < coNestedVariableOccs[x][i + 1]) {
                        current += g(xMin, xMax, alphaDoublePrime, betaDoublePrime) - (alphaDoublePrime ? 1 : 0);
                        maxValue = max(maxValue, current);
                    }  else if (coNestedVariableOccs[x][i] < coNestedVariableOccs[xMin][0] && 
                                coNestedVariableOccs[xMax].back() < coNestedVariableOccs[x][i + 1]) {
                        current += g(xMin, xMax, alphaDoublePrime, betaDoublePrime);
                        maxValue = max(maxValue, current);
                    }
                }
            }
        }
    }
    return maxValue;
}


// Recursive function to compute f^\epsilon(x, alpha, beta)_{1, degree(x)}
int computeF1DegreeX(int x, bool epsilon, bool alpha, bool beta, int start, int end){
    if (start == end - 1) {
        return computeFiiPlus1(x, epsilon, alpha, beta, start);
    }

    int maxValue = INT_MIN;
    for (bool alphaPrime : {true, false}){
        for (bool betaPrime : {true, false}){
            int current = computeF1DegreeX(x, epsilon, alpha, beta,start, end - 1) +
                          computeFiiPlus1(x, epsilon, betaPrime, beta, end -1) - 
                          (alphaPrime && betaPrime ? 1 : 0);
            maxValue = max(maxValue, current);
        }
    }
    return maxValue;
}

// Function to compute f(x, alpha, beta)
int f(int x, bool alpha, bool beta) {
    int maxValue = INT_MIN;
    for (bool epsilon: {true,false}) {
        int current = computeF1DegreeX(x, epsilon, alpha, beta, 0, varDegrees[x]);
        maxValue = max(maxValue, current);
    }
    return maxValue;
}

int g(int x, int y, bool alpha, bool beta) {
    if (x == y) {
        return f(x, alpha, beta);
    }

    auto memoKey = make_tuple(x,y,alpha,beta);
    if (memoG.find(memoKey) != memoG.end()) {
        return memoG[memoKey];
    }

    int result = -1; // Placeholder for max value

    for (bool alphaPrime : {true,false}) {
        for(bool betaPrime : {true, false}) {
            int current = f(x,alpha,betaPrime);

            // Find direct successor x' of x in lesswithcurlylinebelow relation
            for (int xPrime = 1; xPrime <= numOfVars; ++xPrime){
                if (x == xPrime || isDirectPredecessorInPred(x, xPrime)) {
                    continue;
                }
                if (coNestedVariableOccs[x].back() == coNestedVariableOccs[xPrime][0]){
                    // Case 1:  x(degree(x)) == x'(1)
                    current += g(xPrime,y,alphaPrime,beta);
                } else if (coNestedVariableOccs[x].back() < coNestedVariableOccs[xPrime][0]) {
                    // Case 2: x(degree(x)) < x'(1)
                    current += g(xPrime,y,alphaPrime,beta);
                }
            }
            result = max(result,current);
        }
    }

    memoG[memoKey] = result;
    return result;
}

void conestedAlgorithm() {

    fillVarOccsArray();
    
    varDegrees.resize(numOfVars + 1);
    fillDegreesforVars();

    int M = 0; // M denotes the number of simultaneously satisfiable clauses
    
    generateLessThanResults();

    set<int> variables;
    for (int i = 1; i <= numOfVars; i++) {
        variables.insert(i);
    }

    vector<vector<int>> X; // X^0, X^1, ..., X^k
    
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

    int x_min = findXMin(X);
    int x_max = findXMax(X);
    
    printf("x_max: %i\n", x_max);
    printf("x_min: %i\n", x_min);
    
    printf("conested cnf: \n");
    for (size_t i = 0; i < coNestedCNF.size(); ++i) {
        printf("Clause %i: ", i);
        for (int x : coNestedCNF[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }

    defineTriangleSets();

    printf("Triangle sets: \n");
    for (const auto& item : triangleSets) {
        printf("Triangle set for %i: ", item.first);
        for (int x : item.second) {
            printf("%i ", x);
        }
        printf("\n");
    }
    /*M = max(g(x_min, x_max,true,true),
           g(x_min, x_max,true,false),
           g(x_min, x_max,false,true),
           g(x_min, x_max,false,false));*/
}