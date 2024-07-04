#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "../../globals.hpp"


vector<vector<int>> orderedCNF;

vector<int> lits;

vector<int> start;

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
    //printorderedCNF();
}

bool isNested() {
    for (int i = 1; i < orderedCNF.size(); i++) {
        for (int j = i + 1; j < orderedCNF.size(); j++) {
            if (doesClauseOneStraddleClauseTwo(orderedCNF[i], orderedCNF[j]) && doesClauseTwoStraddleClauseOne(orderedCNF[i], orderedCNF[j])) {
                printf("The formula is not nested because clause %i and clause %i overlap \n", i, j);
                for (int k = 0; k < orderedCNF[i].size(); k++) {
                    printf("%i ", orderedCNF[i][k]);
                }
                printf("\n");
                for (int k = 0; k < orderedCNF[j].size(); k++) {
                    printf("%i ", orderedCNF[j][k]);
                }
                return false;
            }
        }
    }
    printf("The formula is nested\n");
    return true;
}

bool doesClauseOneStraddleClauseTwo(vector<int> &clauseOne, vector<int> &clauseTwo) {

    for (int first = 0; first < clauseOne.size(); first++) {
        for (int second = 0; second < clauseTwo.size(); second++) {
            for (int third = clauseOne.size() - 1; third > first; third--) {
                if (nestedLessCompare(clauseOne[first], clauseTwo[second]) && nestedLessCompare(clauseTwo[second], clauseOne[third])) {
                    if (first == third) continue; // Ensure 'first' and 'third' are distinct

                    // Uncomment the below lines to debug or understand the flow
                    // printf("Clause 1 straddles Clause 2 because: \n");
                    // printf("First: %i < Second: %i < Third: %i \n", clauseOne[first], clauseTwo[second], clauseOne[third]);
                    return true;
                }
            }
        }
    }
    //printf("Clause 1 does not straddle Clause 2\n");
    return false; // If no such combination is found

    
}

// This function is redundant and can be removed -- currently for debug purposes
bool doesClauseTwoStraddleClauseOne(vector<int> &clauseOne, vector<int> &clauseTwo) {

    for (int first = 0; first < clauseTwo.size(); ++first) {
        for (int second = 0; second < clauseOne.size(); ++second) {
            for (int third = clauseTwo.size() -1 ; third > first; third--) {
                if (first == third) continue; // Ensure 'first' and 'third' are distinct
                
                if (nestedLessCompare(clauseTwo[first], clauseOne[second]) && nestedLessCompare(clauseOne[second], clauseTwo[third])) {
                    // Uncomment the below lines to debug or understand the flow
                    // printf("Clause 2 straddles Clause 1 because: \n");
                    // printf("First: %i < Second: %i < Third: %i \n", clauseTwo[first], clauseOne[second], clauseTwo[third]);
                    return true;
                }
            }
        }
    }
    //printf("Clause 2 does not straddle Clause 1\n");
    return false;
}

void fillLiteralsAndStart() {
    int startcounter = 0;
    for (int i = 1; i < orderedCNF.size(); i++) {
        start.push_back(startcounter);
        startcounter += orderedCNF[i].size();
        for (int j = 0; j < orderedCNF[i].size(); j++) {
            lits.push_back(orderedCNF[i][j]);
        }
    }

    for (auto elem: lits) {
        printf("%i ", elem);
    }
    printf("\n");

    for(auto elem: start) {
        printf("%i ", elem);
    }
    printf("\n");
}