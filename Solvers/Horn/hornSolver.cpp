#include "../../globals.hpp"

void const printHornClauses(bool horn) {   // Print the Horn clauses for debugging purposes

    if (!horn) {
        printf("The formula is not Horn becasuse clause(s): ");
        int hornCounter = 0;
        for (int i = 1; i < numOfClauses + 1; i++){

            if(!clauses[i].getIsHorn()) {
                printf("%i ", i);
                hornCounter++;
            }
        }
        if (hornCounter == 1) {
            printf("is not Horn.\n");
        } else {
            printf("are not Horn.\n");
        }
    } else {
        printf("The formula is Horn because all clauses are Horn.\n");
    }
    printf("\n\n");
}


bool const isHornFormula(int numOfClauses, vector<Clause> clauses){ // Check if the formula is Horn

    int counter = 0;

    for (int i = 1; i <= numOfClauses; i++) { // Check if all clauses are Horn
        bool isClauseHorn = clauses[i].IsClauseHorn(i);
        
        if (isClauseHorn) counter++;
    }

    if (counter == numOfClauses) {
        return true;
    } else {
        return false;
    }
}


int hornSolver() {

    for (int i = 1; i <= numOfClauses; i++) {  // for each clause
        if (clauses[i].getElemsSize() == 1) { // if it is a unit clause
            if (cnf[i][0] > 0){ // if it is a positive literal
                vars[cnf[i][0]].setValue(TRUE); // set the variable to true
            } else {
                if (vars[abs(cnf[i][0])].getValue() == FREE) // if the variable is free
                    vars[abs(cnf[i][0])].setValue(FALSE); // set the negative unit variable to false
            }
        } else {
            for (int j = 0; j < clauses[i].getElemsSize(); j++) { // for each literal in the non-unit clause
                if(vars[abs(cnf[i][j])].getValue() == FREE) // if the variable is free
                    vars[abs(cnf[i][j])].setValue(FALSE); // set the variable to false
            }
        }
    }

    return 0;
}


 