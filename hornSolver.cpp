#include "globals.hpp"

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


 