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

vector<vector<int>> X; // X^0, X^1, ..., X^k

int CNnumOfVars = 0;
int CNnumOfClauses = 0;

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


pair<int,bool> removeLitsOccurringOnce() {
    unordered_set<int> clausesToBeRemoved;
    bool removedClauses = false;
    int removedLitsNumber = 0;
    int removedClausesNumber = 0;

    for (int i = 1; i <= numOfVars; i++) {
        if (coNestedVariableOccs[i].size() == 1) {
            printf("var Occs size for  %i= %i \n", i, coNestedVariableOccs[i].size());
            int clauseToBeRemoved = 0;
            printf("Removing lit %i\n", i);
            if (!vars[i].getPositiveOccurrances().empty()) {
                clauseToBeRemoved = *vars[i].getPositiveOccurrances().begin();
            } else if (!vars[i].getNegativeOccurrances().empty()) {
                clauseToBeRemoved = *vars[i].getNegativeOccurrances().begin();
            }

            // Insert the clause to be removed into the set
            clausesToBeRemoved.insert(clauseToBeRemoved);
            removedLitsNumber++;
            removedClauses = true;
        }
    }

    // Create a new CNF without the removed clauses
    vector<vector<int>> newCoNestedCNF;
    for (int i = 0; i < coNestedCNF.size(); i++) {
        if (clausesToBeRemoved.find(i) == clausesToBeRemoved.end()) {
            printf("Adding clause %i\n", i);
            newCoNestedCNF.push_back(coNestedCNF[i]);
        } else {
            removedClausesNumber++;
        }
    }

    // Update the global CNF
    coNestedCNF = newCoNestedCNF;

    // Update the number of clauses
    numOfClauses = coNestedCNF.size();

    CNnumOfClauses -= removedClausesNumber;
    CNnumOfVars -= removedLitsNumber;

    return {removedLitsNumber, removedClauses};
}

bool coNestedLessThanCompare(int a, int b) {
    if (coNestedVariableOccs[a].back() <= coNestedVariableOccs[b][0]){
        return true;
    } 
    return false;
}

void generateLessThanResults() {
    for (int i = 1; i <= CNnumOfVars; i++) {
        for (int j = 1; j <= CNnumOfVars; j++) {
            if (i != j) {
                if (coNestedVariableOccs[i].empty() || coNestedVariableOccs[j].empty()) {
                    continue;
                } else {
                    bool result = coNestedLessThanCompare(i,j);
                    coNestedLessThanResArr.push_back({{i,j},result});
                }
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
    vector<vector<int>> emptyVec;
    coNestedVariableOccs = emptyVec;
    coNestedVariableOccs.resize(numOfVars + 1);

    for (int clauseIndex = 1; clauseIndex < coNestedCNF.size(); clauseIndex++) {
        const vector<int>& clause = coNestedCNF[clauseIndex];
        for (int literal : clause) {
            int var = abs(literal);
            coNestedVariableOccs[var].push_back(clauseIndex);
        }
    }

    for (int i = 1; i <= CNnumOfVars; i++) {
        coNestedVariableOccs[i].erase(unique(coNestedVariableOccs[i].begin(), coNestedVariableOccs[i].end()), coNestedVariableOccs[i].end());
    }
}

void fillDegreesforVars() {
    for (int i = 1; i <= numOfVars; i++) {
        if (coNestedVariableOccs[i].empty()) {
            continue;
        }
        varDegrees[i] = coNestedVariableOccs[i].size();
    }
}

bool coNestedPrecedesCompare(int a, int b) {
    // x \\precedes y if x(1) <= y(1) and x(degree(x)) <= y(degree(y))
    printf("Checking for a: %i, b: %i: n", a, b);
    if ((coNestedVariableOccs[b][0] <= coNestedVariableOccs[a][0]) && 
        (coNestedVariableOccs[a].back() <= coNestedVariableOccs[b].back())) {
            printf("True\n");
            return true;
    }
    printf("False\n");
    return false;
}

bool isDirectPredecessorInLess(int a, int b) {

    if (coNestedLessThanCompare(a, b)) {
        printf("Checking if %d is a direct predecessor of %d\n", a, b);
        for (int z = 1; z <= numOfVars; z++) {
            if (z != a && z != b && !coNestedVariableOccs[z].empty() && 
                coNestedLessThanCompare(a, z) && coNestedLessThanCompare(z, b)) {
                printf("Intermediate variable %d found between %d and %d\n", z, a, b);
                return false; // Intermediate variable found
            }
        }
        printf("%d is a direct predecessor of %d\n", a, b);
        return true; // No intermediate variable found, a is a direct predecessor of b
    }
    return false; // a is not less than b
}

bool isDirectPredecessorInPred(int a, int b) {
    if (coNestedPrecedesCompare(a, b)) {
        for (int z = 1; z <= numOfVars; z++) {
            if (coNestedVariableOccs[z].empty()) {
                continue;
            }
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
        int count = 0;
        bool isPrecMaximal = true;
        for (int y : variables) {
            if (x != y){
                if(!coNestedPrecedesCompare(y, x)) {
                    printf("%i ", x);
                    count++;
                    isPrecMaximal = false;
                }  
            }
            
        }
        if (isPrecMaximal) {
            printf("Count for %i: %i\n", count);
            maximalElements.push_back(x);
        }
        printf("\n");
    }
    return maximalElements;
}

// Equivalence relation
bool coNestedLessThanWithCurlyLineBelow(int x, int y, const vector<vector<int>>& levels) {
    printf("Checking if %d is less than %d with curly line below.\n", x, y);
    int xlevel = -1;
    int ylevel = -1;

    // Determine levels for x and y
    for (size_t k = 0; k < levels.size(); ++k) {
        if (find(levels[k].begin(), levels[k].end(), x) != levels[k].end()) {
            xlevel = k;
        }
        if (find(levels[k].begin(), levels[k].end(), y) != levels[k].end()) {
            ylevel = k;
        }
    }

    printf("x: %d, y: %d, xlevel: %d, ylevel: %d\n", x, y, xlevel, ylevel);

    // Check if x and y are on the same level and valid levels are found
    if (xlevel != ylevel || xlevel == -1 || ylevel == -1) {
        printf("Not on the same level or invalid levels.\n");
        return false;
    }

     // Check if x < y
    if (!coNestedLessThanCompare(x, y)) {
        printf("x %i is not less than y. %i\n",x,y);
        return false;
    }

    // If both are on level 0, return true since they are comparable and there's no intermediate level
    if (xlevel == 0 && ylevel == 0) {
        return true;
    }

    // Check for the intermediate variable z in the level just below xlevel
    const vector<int>& sublevel = levels[xlevel - 1];
    for (int z : sublevel) {
        for (int i = 0; i < varDegrees[z]; ++i) {
            if (coNestedVariableOccs[x].back() <= coNestedVariableOccs[z][i] 
                && coNestedVariableOccs[z][i] <= coNestedVariableOccs[y][0]) {
                printf("Intermediate variable z: %d found making the condition false.\n", z);
                return false;
            }
        }
    }   
    
    return true;
}
pair<int, int> findMinMaxElements(const set<int>& variables, const vector<vector<int>>& levels) {
    int minElement = -1;
    int maxElement = -1;

    printf("TESTSTEST\n");

    for (int x : variables) {
        bool isMinimal = true;
        bool isMaximal = true;

        for (int y : variables) {
            if (x != y) {
                if (!coNestedLessThanWithCurlyLineBelow(x, y, levels)) {
                    isMinimal = false;
                }
                if (coNestedLessThanWithCurlyLineBelow(y, x, levels)) {
                    isMaximal = false;
                }
            }
        }

        if (isMinimal && (minElement == -1 || coNestedLessThanCompare(x, minElement))) {
            minElement = x;
            printf("minElement: %i\n", minElement);
        }

        if (isMaximal && (maxElement == -1 || coNestedLessThanCompare(maxElement, x))) {
            maxElement = x;
            printf("maxElement: %i\n", maxElement);
        }
    }

    return {minElement, maxElement};
}

set<int> defineTriangleSets(int x, int i, int j) {

    int startClause = coNestedVariableOccs[x][i-1];
    int endClause = coNestedVariableOccs[x][j-1];

    set<int> triangleSet;

    printf("cnf: \n");
    for (int i = 0; i < coNestedCNF.size(); i++) {
        for (int x : coNestedCNF[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }

    for (int clauseIDx = startClause; clauseIDx <= endClause; clauseIDx++) {
        for (int lit : coNestedCNF[clauseIDx]) {
        int absLit = abs(lit);
        //printf("%i ", absLit);
        triangleSet.insert(absLit);
        }
    }   
    printf("The triangle set for %i with i %i and j %i is: ", x, i, j);
    for (int x : triangleSet) {
        printf("%i ", x);
    }
    printf("\n");
    return triangleSet;
}

// Function to find the minimal and maximal elements in X^0 regarding the < relation
pair<int, int> findMinMaxInX0(const vector<vector<int>> &X) {
    int x_min = X[0][0];
    int x_max = X[0].back();

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

    bool clause1Condition = (find(coNestedCNF[startClause].begin(), coNestedCNF[startClause].end(), x) != coNestedCNF[startClause].end() ||
                             find(coNestedCNF[startClause].begin(), coNestedCNF[startClause].end(), -x) != coNestedCNF[startClause].end());
    bool clause2Condition = (find(coNestedCNF[endClause].begin(), coNestedCNF[endClause].end(), x) != coNestedCNF[endClause].end() || 
                             find(coNestedCNF[endClause].begin(), coNestedCNF[endClause].end(), -x) != coNestedCNF[endClause].end());

    if ((clause1Condition && epsilon != alpha) || (!clause1Condition && epsilon == alpha) ||
        (clause2Condition && epsilon != beta) || (!clause2Condition && epsilon == beta)) {
        printf("Returning -100\n");
        return -100;
    } else {
        printf("Returning %i\n", (alpha ? 1 : 0) + (beta ? 1 : 0));
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

// Function to compute f(x, alpha, beta)
int f(int x, bool alpha, bool beta) {
    int g(int x, int y, bool alpha, bool beta); // Forward declaration

    int currentMax = 0;

    for (int i = 1; i < varDegrees[x] - 1; i++) {
        for (int j = i + 1; j <= varDegrees[x]; j++) {
            set<int> triangleSet = defineTriangleSets(x, i, j);
            if (triangleSet.empty()) {
                // if X(X,i) is empty, then f(x, alpha, beta) = theta(epsilon, alpha, beta)_i
                // i.e. no need to look for hills
                for (bool epsilon : {true, false}) {
                    for (bool alpha : {true, false}) {
                        for (bool beta : {true, false}) {
                            int result = computeThetaEpsilon(x, epsilon, alpha, beta, i);
                            if (result > currentMax) {
                                currentMax = result;
                            }
                        }
                    }
                }
            } else {
                // we are checking hills
                // if X(X,i) is not empty, then f(x, alpha, beta) = max_{epsilon, alpha, beta} f^\epsilon(x, alpha, beta)_{i, i+1}
                // which has 4 cases
                // the first elem of minMax is minimal element and the second is maximal element
                pair<int, int> minMax = findMinMaxElements(triangleSet,X);
                printf("triangle set: ");
                for (int x : triangleSet) {
                    printf("%i ", x);
                }
                printf("minMax: %i, %i\n", minMax.first, minMax.second);
                for (bool alphaPrime : {true, false}) {
                    for (bool alphaDoublePrime : {true, false}) {
                        for (bool betaPrime : {true, false}) {
                            for (bool betaDoublePrime : {true, false}) {
                                if (coNestedVariableOccs[x][i-1] == coNestedVariableOccs[minMax.first][0] && 
                                    coNestedVariableOccs[minMax.second].back() == coNestedVariableOccs[x][i]) {
                                    int result =  g(minMax.first, minMax.second, alphaDoublePrime, betaDoublePrime) -
                                                  (alphaDoublePrime && alphaPrime ? 1 : 0) -
                                                  (betaDoublePrime && betaPrime ? 1 : 0);
                                    if (result > currentMax) {
                                        currentMax = result;
                                    }
                                } else if (coNestedVariableOccs[x][i-1] < coNestedVariableOccs[minMax.first][0] && 
                                           coNestedVariableOccs[minMax.second].back() == coNestedVariableOccs[x][i]){
                                    int result = g(minMax.first, minMax.second, alphaPrime, betaDoublePrime) -
                                                  (betaPrime && betaDoublePrime ? 1 : 0);
                                    if (result > currentMax) {
                                        currentMax = result;
                                    }
                                } else if (coNestedVariableOccs[x][i-1] == coNestedVariableOccs[minMax.first][0] &&
                                           coNestedVariableOccs[minMax.second].back() < coNestedVariableOccs[x][i]) {
                                    int result = g(minMax.first, minMax.second, alphaDoublePrime, betaPrime) - 
                                                 (alphaPrime && alphaDoublePrime ? 1 : 0);
                                    if (result > currentMax) {
                                        currentMax = result;
                                    }
                                }  else if (coNestedVariableOccs[x][i-1] < coNestedVariableOccs[minMax.first][0] && 
                                            coNestedVariableOccs[minMax.second].back() < coNestedVariableOccs[x][i]) {
                                    int result = g(minMax.first, minMax.second, alphaDoublePrime, betaDoublePrime);
                                    if (result > currentMax) {
                                        currentMax = result;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return currentMax;
}

int g(int x, int y, bool alpha, bool beta) {
    if (x == y) {
        printf("x == y\n");
        printf("Call to f(%i, %i, %i)\n", x, alpha, beta);
        int res = f(x, alpha, beta);
        printf("Result: %i\n", res);
        return res;
        
    }

    auto memoKey = make_tuple(x,y,alpha,beta);
    if (memoG.find(memoKey) != memoG.end()) {
        return memoG[memoKey];
    }

    int result = -1; // Placeholder for max value

    for (bool alphaPrime : {true,false}) {
        for(bool betaPrime : {true, false}) {
            int case1CurrentMax = 0;
            int case2CurrentMax = 0;
            int case1Current = 0;
            int case2Current = 0;

            // Find direct predecessor x' of x in lesswithcurlylinebelow relation
            for (int xPrime = 1; xPrime <= numOfVars; ++xPrime){
                if (x == xPrime || coNestedVariableOccs[xPrime].empty()) {
                    continue;
                }
                if (isDirectPredecessorInLess(x, xPrime)) {
                    if (coNestedLessThanWithCurlyLineBelow(xPrime, y, X)) {
                        if (coNestedVariableOccs[x].back() == coNestedVariableOccs[xPrime][0]) {
                            // Case 1:  x(degree(x)) == x'(1)
                            printf("Case 1\n");
                            printf("x: %i, xPrime: %i\n", x, xPrime);
                            case1Current = f(x, alphaPrime, beta) + 
                                           g(xPrime, y, alpha, betaPrime) -
                                           (alphaPrime && betaPrime ? 1 : 0);

                            if (case1Current > case1CurrentMax) {
                                case1CurrentMax = case1Current;
                            }
                        } else if (coNestedVariableOccs[x].back() < coNestedVariableOccs[xPrime][0]) {
                            // Case 2: x(degree(x)) < x'(1)
                            printf("Case 2\n");
                            printf("x: %i, xPrime: %i\n", x, xPrime);
                            case2Current = f(x, alpha, betaPrime) +
                                           g(xPrime, y, alphaPrime, beta);

                            if (case2Current > case2CurrentMax) {
                                case2CurrentMax = case2Current;
                            }
                        }
                    }
                }
            }
            result = max(case1CurrentMax,max(case2CurrentMax,result));
        }
    }

    memoG[memoKey] = result;
    return result;
}

void conestedAlgorithm() {

    CNnumOfClauses = numOfClauses;
    CNnumOfVars = numOfVars;

    int M = 0; // M denotes the number of simultaneously satisfiable clauses

    //Lemma 2.1: If var degree = 1, then theta' = theta - c_(x(1)) and M(theta) = M(theta') + 1
    // Therefore the clause is deleted from the cnf
    bool removedelem = true;
    while (removedelem)
    {   
        fillVarOccsArray();
        pair<int,bool> removedElem = removeLitsOccurringOnce();
        M += removedElem.first;
        printf("M %i\n", M);
        removedelem = removedElem.second;
        if (M == numOfClauses)
        {   
            printf("M %i == numOfClauses %i\n", M, numOfClauses);
            return;
        }
        
    }

    varDegrees.resize(numOfVars + 1);
    fillDegreesforVars();

    generateLessThanResults();

    set<int> variables;
    for (int i = 1; i <= numOfVars; i++) {
        if (!coNestedVariableOccs[i].empty()) {
            variables.insert(i);
        }
    }
    printf("cnf: \n");
    for (int i = 0; i < coNestedCNF.size(); i++) {
        for (int x : coNestedCNF[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }
    while (!variables.empty()) {
        vector<int> Xk = findPrecMaximalElements(variables);

        if (Xk.empty()) {
            // Handling empty Xk scenario
            // If no prec-maximal elements are found, then they must be in the same equivalent relation
            for (int x : variables) {
                Xk.push_back(x);
            }
            
        }
        X.push_back(Xk);
        for (int x : Xk) {
            variables.erase(x);
        }
    }

    printf("X: \n");
    for (int i = 0; i < X.size(); i++) {
        printf("X^%i:",i);
        for (int x : X[i]) {
            printf("%i ", x);
        }
        printf("\n");
    }
    
    auto [x_min, x_max] = findMinMaxInX0(X);
    printf("x_min: %i, x_max: %i\n", x_min, x_max);

    int result1 = g(1, 1, true, true);
    int result2 = g(x_min, x_max, true, false);
    int result3 = g(x_min, x_max, false, true);
    int result4 = g(x_min, x_max, false, false);

    M += max({g(x_min, x_max,true,true),
           g(x_min, x_max,true,false),
           g(x_min, x_max,false,true),
           g(x_min, x_max,false,false)});

    printf("M %i\n",M);
    printf("Result 1: %i\n", result1);
    printf("Result 2: %i\n", result2);
    printf("Result 3: %i\n", result3);
    printf("Result 4: %i\n", result4);

}