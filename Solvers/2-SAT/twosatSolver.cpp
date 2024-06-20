#include "../../globals.hpp"

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