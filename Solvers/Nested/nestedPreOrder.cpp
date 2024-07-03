#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "../../globals.hpp"


vector<vector<int>> orderedCNF;

struct nestedLess {
    bool operator()(int a, int b) const {
        return abs(a) < abs(b);
    }
};

nestedLess nestedLessCompare;

vector<int> nestedClausePreOrder(vector<int> &clause) {
    std::vector<int> preOrder(clause.size());
    preOrder = clause;
    std::sort(preOrder.begin(), preOrder.end(), nestedLess());

    for(auto elem: clause){
        printf("%i ", elem);
    }
    printf(" || ");

    for(auto elem: preOrder){
        printf("%i ", elem);
    }
    printf("\n");

    return preOrder;
}

void printorderedCNF() {
    for (auto clause: orderedCNF) {
        for (auto elem: clause) {
            printf("%i ", elem);
        }
        printf("\n");
    }
}

void createOrderedCNF() {
    for (auto clause: cnf) {
        orderedCNF.push_back(nestedClausePreOrder(clause));
    }
    printorderedCNF();
}

bool doesClauseOneStraddleClauseTwo(vector<int> &clauseOne, vector<int> &clauseTwo) {
    vector<int> preOrderOne = nestedClausePreOrder(clauseOne);
    vector<int> preOrderTwo = nestedClausePreOrder(clauseTwo);

    int first = 0;
    int second = 0;
    int third = clauseOne.size() - 1;
    int compareCount = 0;
    while (compareCount != std::floor(clauseOne.size()/2)) {
        if (nestedLessCompare(preOrderOne[first], preOrderTwo[second]) && nestedLessCompare(preOrderTwo[second], preOrderOne[third])) {
            printf("Clasue 1 straddles Clause 2 because: \n");
            printf("First: %i < Second: %i < Third: %i \n", preOrderOne[first], preOrderTwo[second], preOrderOne[third]);
            return true;
        } else {
            first++;
            second++;
            third--;
            compareCount++;
        }
    }

    printf("Clause 1 does not straddle Clause 2\n");
    return false;
}

bool doesClauseTwoStraddleClauseOne(vector<int> &clauseOne, vector<int> &clauseTwo) {
    vector<int> preOrderOne = nestedClausePreOrder(clauseOne);
    vector<int> preOrderTwo = nestedClausePreOrder(clauseTwo);

    int first = 0;
    int second = 0;
    int third = clauseTwo.size() - 1;
    int compareCount = 0;
    while (compareCount != std::floor(clauseTwo.size()/2)) {
        if (nestedLessCompare(preOrderTwo[first], preOrderOne[second]) && nestedLessCompare(preOrderOne[second], preOrderTwo[third])) {
            printf("Clasue 2 straddles Clause 1 because: \n");
            printf("First: %i < Second: %i < Third: %i \n", preOrderTwo[first], preOrderOne[second], preOrderTwo[third]);
            return true;
        } else {
            first++;
            second++;
            third--;
            compareCount++;
        }
    }

    printf("Clause 2 does not straddle Clause 1\n");
    return false;
}

