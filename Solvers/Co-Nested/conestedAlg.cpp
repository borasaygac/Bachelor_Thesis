#include "../../globals.hpp"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <tuple>


vector<pair<pair<int,int>, bool>> coNestedLessThanResArr;

vector<vector<int>> coNestedVariableOccs;

vector<int> varDegrees;

unordered_map<int, set<int>> triangleSets;

namespace std {
    template <>
    struct hash<tuple<int, int, bool, bool>> {
        size_t operator()(const tuple<int, int, bool, bool>& t) const {
            auto hash1 = hash<int>{}(get<0>(t));
            auto hash2 = hash<int>{}(get<1>(t));
            auto hash3 = hash<bool>{}(get<2>(t));
            auto hash4 = hash<bool>{}(get<3>(t));
            return ((hash1 ^ (hash2 << 1)) >> 1) ^ ((hash3 ^ (hash4 << 1)) >> 1);
        }
    };

    template <>
    struct hash<tuple<int, bool, bool>> {
        size_t operator()(const tuple<int, bool, bool>& t) const {
            auto hash1 = hash<int>{}(get<0>(t));
            auto hash2 = hash<bool>{}(get<1>(t));
            auto hash3 = hash<bool>{}(get<2>(t));
            return ((hash1 ^ (hash2 << 1)) >> 1) ^ (hash3 << 1);
        }
    };
}

unordered_map<tuple<int,int,bool,bool>, int> memoG;
unordered_map<tuple<int,bool,bool>, int> memoF;

int removeLitsOccurringOnce() {
    unordered_set<int> clausesToBeRemoved;
    int removedLitsNumber = 0;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].getTotalOccurances() == 1) {
            int clauseToBeRemoved = 0;
            printf("Removing lit %i", i);
            if (!vars[i].getPositiveOccurrances().empty()) {
                clauseToBeRemoved = *vars[i].getPositiveOccurrances().begin();
            } else if (!vars[i].getNegativeOccurrances().empty()) {
                clauseToBeRemoved = *vars[i].getNegativeOccurrances().begin();
            }

            // Insert the clause to be removed into the set
            clausesToBeRemoved.insert(clauseToBeRemoved);
            removedLitsNumber++;
        }
    }

    // Create a new CNF without the removed clauses
    vector<vector<int>> newCoNestedCNF;
    for (int i = 0; i < coNestedCNF.size(); i++) {
        if (clausesToBeRemoved.find(i) == clausesToBeRemoved.end()) {
            newCoNestedCNF.push_back(coNestedCNF[i]);
        }
    }

    // Update the global CNF
    coNestedCNF = newCoNestedCNF;

    return removedLitsNumber;
}

bool coNestedLessThanCompare(int a, int b) {
    if (coNestedVariableOccs[a].back() <= coNestedVariableOccs[b][0]){
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

    for (int clauseIndex = 1; clauseIndex <= coNestedCNF.size(); clauseIndex++) {
        const vector<int>& clause = coNestedCNF[clauseIndex];
        for (int literal : clause) {
            int var = abs(literal);
            coNestedVariableOccs[var].push_back(clauseIndex);
        }
    }

    for (int i = 1; i <= numOfVars; i++) {
        coNestedVariableOccs[i].erase(unique(coNestedVariableOccs[i].begin(), coNestedVariableOccs[i].end()), coNestedVariableOccs[i].end());
    }
}

void fillDegreesforVars() {
    for (int i = 1; i <= numOfVars; i++) {
        varDegrees[i] = vars[i].getTotalOccurances();
    }
}

bool coNestedPrecedesCompare(int a, int b) {
    printf("Checking for a: %i, b: %i: n", a, b);
    if ((coNestedVariableOccs[a][0] <= coNestedVariableOccs[b][0]) && 
        (coNestedVariableOccs[b].back() <= coNestedVariableOccs[a].back())) {
            printf("True\n");
            return true;
    }
    printf("False\n");
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
            if (x != y){
                if(coNestedPrecedesCompare(x, y)) {
                    isPrecMaximal = false;
                    break;
                }  
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

    printf("clause occs:\n");
    for (int i = 1; i <= numOfVars; i++) {
        printf("Variable %i: ", i);
        for (int occ : coNestedVariableOccs[i]) {
            printf("%i ", occ);
        }
        printf("\n");
    }
    for (int i = 1; i <= numOfVars; i++) {
        int varDegree = varDegrees[i];
        int startClause = coNestedVariableOccs[i][0];
        for (int j = 0; j < varDegree - 1; j++) {
            int endClause = coNestedVariableOccs[i][j+1];
            printf("for variable %i with j %i, startClause: %i, endClause: %i\n", i, j, startClause, endClause);
            
            set<int> triangleSet;

            for (int clauseIDx = startClause; clauseIDx < endClause; clauseIDx++) {
                printf("Considering clause %i: ", clauseIDx);
                for (int lit : coNestedCNF[clauseIDx]) {
                    int absLit = abs(lit);
                    printf("%i ", absLit);
                    triangleSet.insert(absLit);
                }
                printf("\n");
            }

            triangleSets[i * 100 + endClause] = triangleSet; 
        }
    }
}

// Function to find the minimal and maximal elements in X^0 regarding the < relation
pair<int, int> findMinMaxInX0(const vector<vector<int>> &X) {
    int x_min = X[0][0];
    int x_max = X[0][0];

    for (int x : X[0]) {
        if (coNestedLessThanCompare(x, x_min)) {
            x_min = x;
        }
        if (coNestedLessThanCompare(x_max, x)) {
            x_max = x;
        }
    }
    return {x_min, x_max};
}

// Function to compute theta^\epsilon(x, alpha, beta)_i
int computeThetaEpsilon(int x, bool epsilon, bool alpha, bool beta, int i) {
    // Lemma 1 and 2 -- no var can occur only once. Then m+1 anyways so we need to change it accordingly
    int startClause = coNestedVariableOccs[x][i];
    int endClause = coNestedVariableOccs[x][i + 1];

    bool clause1Condition = (find(coNestedCNF[startClause].begin(), coNestedCNF[startClause].end(), x) != coNestedCNF[startClause].end());
    bool clause2Condition = (find(coNestedCNF[endClause].begin(), coNestedCNF[endClause].end(), x) != coNestedCNF[endClause].end());

    if ((clause1Condition && epsilon != alpha) || (!clause1Condition && epsilon == alpha) ||
        (clause2Condition && epsilon != beta) || (!clause2Condition && epsilon == beta)) {
        return INT_MIN;
    } else {
        return (alpha ? 1 : 0) + (beta ? 1 : 0);
    }
}



// Function to find minimal and maximal variables in X(x, i)
pair<int, int> findMinMaxInTriangle(int x, int i) {
    set<int> triangleSet = triangleSets[x * 100 + i];
    int xMin = *min_element(triangleSet.begin(), triangleSet.end());
    int xMax = *max_element(triangleSet.begin(), triangleSet.end());
    return {xMin, xMax};
}


// Function to compute f^\epsilon(x, alpha, beta)_ii+1
int computeFiiPlus1(int x, bool epsilon, bool alpha, bool beta, int i) {
    int thetaEpsilon = computeThetaEpsilon(x, epsilon, alpha, beta, i);
    int g(int x, int y, bool alpha, bool beta); // Forward declaration

    auto [xMin, xMax] = findMinMaxInTriangle(x,i);
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

                    if (coNestedVariableOccs[x][i-1] == coNestedVariableOccs[xMin][0] && 
                        coNestedVariableOccs[xMax].back() == coNestedVariableOccs[x][i]) {
                        current +=  g(xMin, xMax, alphaDoublePrime, betaDoublePrime) -
                                     (alphaDoublePrime && alphaPrime ? 1 : 0) -
                                     (betaDoublePrime && betaPrime ? 1 : 0);
                        maxValue = max(maxValue,current);
                    } else if (coNestedVariableOccs[x][i-1] < coNestedVariableOccs[xMin][0] && 
                               coNestedVariableOccs[xMax].back() == coNestedVariableOccs[x][i]){
                        current += g(xMin, xMax, alphaPrime, betaDoublePrime) -
                                    (betaPrime && betaDoublePrime ? 1 : 0);
                        maxValue = max(maxValue, current);
                    } else if (coNestedVariableOccs[x][i-1] == coNestedVariableOccs[xMin][0] &&
                               coNestedVariableOccs[xMax].back() < coNestedVariableOccs[x][i]) {
                        current += g(xMin, xMax, alphaDoublePrime, betaPrime) - 
                                    (alphaPrime && alphaDoublePrime ? 1 : 0);
                        maxValue = max(maxValue, current);
                    }  else if (coNestedVariableOccs[x][i-1] < coNestedVariableOccs[xMin][0] && 
                                coNestedVariableOccs[xMax].back() < coNestedVariableOccs[x][i]) {
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

    int M = 0; // M denotes the number of simultaneously satisfiable clauses


    //Lemma 2.1: If var degree = 1, then theta' = theta - c_(x(1)) and M(theta) = M(theta') + 1
    // Therefore the clause is deleted from the cnf
    M += removeLitsOccurringOnce();

    printf("lits removed %i\n",M);
    printf("new CNF\n");
    printf("num of clauses %i\n",numOfClauses - M);
    printf("og cnf size %i\n",cnf.size());
    printf("New cnf size %i\n",coNestedCNF.size());
    for (int i = 0; i <= coNestedCNF.size(); i++){
        printf("Clause %i:",i);
        for (int j = 0; j < coNestedCNF[i].size(); j++) {
            printf("%i ", coNestedCNF[i][j]);
        }
        printf("\n");
    }
    

    fillVarOccsArray();

    
    
    varDegrees.resize(numOfVars + 1);
    fillDegreesforVars();

    
    generateLessThanResults();

    set<int> variables;
    for (int i = 1; i <= numOfVars; i++) {
        variables.insert(i);
    }

    vector<vector<int>> X; // X^0, X^1, ..., X^k

    printf("conested cnf: \n");
    for (size_t i = 0; i < coNestedCNF.size(); ++i) {
        printf("Clause %i: ", i);
        for (int x : coNestedCNF[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }
    
    while (!variables.empty()) {
        vector<int> Xk = findPrecMaximalElements(variables);

        // Debugging: Print the size of variables and Xk
        printf("Size of variables: %zu\n", variables.size());
        printf("Size of Xk: %zu\n", Xk.size());
        if (Xk.empty()) {
            printf("No \\prec-maximal elements found. Possible issue detected.\n");
            // Handling empty Xk scenario
            // If no prec-maximal elements are found, then they must be in the same equivalent relation
            for (int x : variables) {
                printf("Adding %i to Xk\n", x);
                Xk.push_back(x);
            }
            
        }
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
    auto [x_min, x_max] = findMinMaxInX0(X);

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

    M = max({g(x_min, x_max,true,true),
           g(x_min, x_max,true,false),
           g(x_min, x_max,false,true),
           g(x_min, x_max,false,false)});

    printf("M %i\n");

}