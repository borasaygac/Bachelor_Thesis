#include "globals.hpp"


bool isFormulaSat() {
    for (int i = 1; i <= numOfClauses; i++) {
        if (!clauses[i].evaluateClause()) {
            return false;
        }
    }
    return true;
}

