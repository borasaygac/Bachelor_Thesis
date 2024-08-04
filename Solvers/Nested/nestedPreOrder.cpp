#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include "../../globals.hpp"


vector<vector<int>> nestedCNF;

vector<int> lits;

vector<int> start;

vector<int> nextR;

vector<int> startR;

vector<vector<vector<int>>> satTable;

vector<vector<int>> newsat;



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
    for (auto clause: nestedCNF) {
        for (auto elem: clause) {
            printf("%i ", elem);
        }
        printf("\n");
    }
}

void createOrderedCNF() {
    for (auto clause: cnf) {
        nestedCNF.push_back(nestedClausePreOrder(clause));
    }
    //printorderedCNF();
}

bool isNested() {
    for (int i = 1; i < nestedCNF.size(); i++) {
        for (int j = i + 1; j < nestedCNF.size(); j++) {
            if (doesClauseOneStraddleClauseTwo(nestedCNF[i], nestedCNF[j]) && doesClauseOneStraddleClauseTwo(nestedCNF[j], nestedCNF[i])) {
                printf("Nested Recognition: N \nThe formula is not nested because clause %i and clause %i overlap \n", i, j);
                printf("Clause %i: [", i);
                for (int k = 0; k < nestedCNF[i].size(); k++) {
                    printf("%i ", nestedCNF[i][k]);
                }
                printf("]\n");
                printf("Clause %i: [", j);
                for (int k = 0; k < nestedCNF[j].size(); k++) {
                    printf("%i ", nestedCNF[j][k]);
                }
                printf("]\n\n");
                return false;
            }
        }
    }
    printf("Nested Recognition: Y\nReason: No two clauses overlap.\n\n");
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


void fillLiteralsAndStart() {
    int startcounter = 0;
    for (int i = 0; i < nestedCNF.size(); i++) {
        start.push_back(startcounter);
        startcounter += nestedCNF[i].size();
        for (int j = 0; j < nestedCNF[i].size(); j++) {
            lits.push_back(nestedCNF[i][j]);
        }
    }
    // add the extra dummy clause
    start.push_back(startcounter);
    //lits.push_back(0);
}

// refactored nested solver
void modifyNewSatTable(int x) {
    int tmp[2][2];
                for (int s = 0; s < 2; s++) { //init tmp
                    for (int t = 0; t < 2; t++) {
                        tmp[s][t] = max(newsat[s][0] * satTable[x][0][t], 
                                        newsat[s][1] * satTable[x][1][t]);
                    }
                }   

                for (int s = 0; s < 2; s++) { 
                    for (int t = 0; t < 2; t++) {
                        newsat[s][t] = tmp[s][t];
                    }
                }
}

void upgradeFromOneToTwo(int x, int sig) {
    int t = (x == sig) ? 1 : 0;
    for (int s = 0; s < 2; s++) {
        if (newsat[s][t] == 1) newsat[s][t] = 2;
    }
}

void computeNewSatTable(int i, int next[]) {

    int j = start[i];
    int sig = lits[j];
    int x = abs(sig);

    
    newsat[0][0] = 1;
    newsat[1][1] = 1;
    newsat[0][1] = 0;
    newsat[1][0] = 0;

    while(true) {
            if (x == abs(sig)){
                // Upgrade newsat from 1 to 2 if possible
                upgradeFromOneToTwo(x, sig);

                j++;
                sig = lits[j];
                if (j == start[i + 1]) break;
            }
            
            modifyNewSatTable(x);
                // Modify newsat for the next x value
                
            x = next[x];
        }
}

void nestedSolver() {

    int next[numOfVars];
    newsat = vector<vector<int>>(2, vector<int>(2));

    satTable = vector<vector<vector<int>>>(numOfVars + 1, vector<vector<int>>(2, vector<int>(2)));

    for (int i = 0; i <= numOfVars; i++) {
        next[i] = i+1;
    }

    for (int i = 0; i <= numOfVars; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                satTable[i][j][k] = 1;
            }
        }
    }   

    for (int i = 1; i < numOfClauses + 1; i++) {
        int l = abs(lits[start[i]]);
        int r = abs(lits[start[i + 1] - 1]);

        computeNewSatTable(i, next);
        next[l] = r;

        for (int s = 0; s < 2; s++) {
            for (int t = 0; t < 2; t++) {
                satTable[l][s][t] = newsat[s][t] / 2;
            }
        }
    }

    if (satTable[0][1][1] == 1) {
        printf("Nested Algorithm: The formula is satisfiable\n");
    } else {
        printf("Nested Algorithm: The formula is not satisfiable\n");
    }
}

