#include "../../globals.hpp"
#include "../../dpdl.hpp"


bool isTwoSat(int numOfClauses, vector<Clause> clauses) { // Checks whether the formula is 2-SAT

    for (int i = 1; i <= numOfClauses; i++) {
        if (!clauses[i].IsClauseTwoSat(i)) {
            printf("2-SAT Recognition: N\nReason:");
            vector<int> nontwoSATclause;
            for (int j = 0; j < clauses[i].getElemsSize(); j++) {
                nontwoSATclause.push_back(cnf[i][j]);
            }

            printf("Clause %i: [", i);
            for (int j = 0; j < nontwoSATclause.size(); j++) {
                if (j != nontwoSATclause.size() - 1) {
                    printf("%i, ", nontwoSATclause[j]);
                } else {
                    printf("%i", nontwoSATclause[j]);
                }
            }
            printf("] is not 2-SAT.\n");
            printf("\n");
            return false;
        }
    }
    printf("2-SAT Recognition: Y\nReason: all clauses are 2-SAT.\n");
    return true;
}


void* DPLL(void* arg) { // Solves the 2-SAT formula -- main function 
    while (true) {
        propagate();
        chooseINC();
    }
}


void chooseINC() { // Choose the next variable to assign -- Heuristic: incremental
    dc++;
    //printf("curvar: %i\n", curVar);
    while (vars[curVar].getValue() != FREE) curVar++;

    vars[curVar].setValue(TRUE);
    vars[curVar].setForced(false);
    assig.push(curVar);
    update(curVar);
}


