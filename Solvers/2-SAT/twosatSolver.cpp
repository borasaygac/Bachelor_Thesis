#include "../../globals.hpp"
#include "../../dpdl.hpp"


// Checks whether the formula is 2-SAT
bool isTwoSat(int numOfClauses, vector<Clause> clauses) {

    for (int i = 1; i <= numOfClauses; i++) {
        if (!clauses[i].IsClauseTwoSat(i)) {
            printf("The formula is not 2-SAT because:\n");
            vector<int> nontwoSATclause;
            for (int j = 0; j < clauses[i].getElemsSize(); j++) {
                nontwoSATclause.push_back(cnf[i][j]);
            }

            printf("Clause %i: [", i);
            for (int j = 0; j < nontwoSATclause.size(); j++) {
                printf("%i, ", nontwoSATclause[j]);
            }
            printf("] is not 2-SAT.\n");
            printf("\n\n");
            return false;
        }
    }
    printf("The formula is 2-SAT because all clauses are 2-SAT.\n\n\n");
    return true;
}

// Solves the 2-SAT formula -- main function 
void* twoSatDPDL(void* arg) {
    while (true) {
        propagate();
        chooseINC();
    }
}



void chooseINC() {
    dc++;
    //printf("curvar: %i\n", curVar);
    while (vars[curVar].getValue() != FREE) curVar++;

    vars[curVar].setValue(TRUE);
    vars[curVar].setForced(false);
    assig.push(curVar);
    update(curVar);
}


