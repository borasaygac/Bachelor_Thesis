#include "../../globals.hpp"
#include "../../dpdl.hpp"

#include <set>
#include <algorithm>
#include <iostream>

void update(int assertedVar) {
    // clauses where assertedVar evaluates to FALSE
    std::set<int>* clausesToUpdate;

    // clauses where assertedVar evaluates to TRUE
    std::set<int>* clausesToMarkSatisfied;

    // In DPLL - analogous to UP - if the var is true than the negative occurrances need to be updated as they will evaluate to false
    // vice versa otherwise

    clausesToUpdate = (vars[assertedVar].getValue() == TRUE) ? &vars[assertedVar].getNegativeOccurrances(): 
                                                                &vars[assertedVar].getPositiveOccurrances();

    // Similarly, the clauses to mark satisfied are the ones are the positives if the var is true, and vice versa 
    if (vars[assertedVar].getValue() == TRUE) {
        clausesToMarkSatisfied = &vars[assertedVar].getDynamicPositiveOccurrances();
    } else {
        clausesToMarkSatisfied = &vars[assertedVar].getDynamicNegativeOccurrances();      
        assertedVar = -assertedVar;
    }

    std::set<int> copy = *clausesToMarkSatisfied;

    // While clauses to mark satisfied are unsatisfied, mark satisfied and
    // erase all references of the literals occuring in the clause, since they can be disregarded
    for (auto clauseIndex = copy.begin(); clauseIndex != copy.end(); ++clauseIndex) {
        Clause* clause = &clauses[*clauseIndex];
        clause->setSatisfiedBy(assertedVar);

        for (int i = 0; i < clause->literals.size(); i++) {
            auto var = &vars[std::abs(clause->literals[i])];

            // remove the clause from the dynamic occurrances of the variable according to the sign of the literal
            clause -> literals[i] > 0 ? var->getDynamicPositiveOccurrances().erase(*clauseIndex): 
                                        var->getDynamicNegativeOccurrances().erase(*clauseIndex);

            // detect pure literals
            /* if ((*var).getValue() == FREE && !(*var).getEnqueued()) {
                if ((*var).getDynamicPositiveOccurrances().size() == 0 && (*var).getDynamicNegativeOccurrances().size() > 0) {
                    int index = clause->getElems().at(i)->getIndex();
                    std::cout << "Index: " << index << std::endl;
                    toPropagate.push(-std::abs(clause->literals[i]));
                    (*var).setEnqueued(true);
                }
                if ((*var).getDynamicNegativeOccurrances().size() == 0 && (*var).getDynamicPositiveOccurrances().size() > 0) {
                    toPropagate.push(std::abs(clause->literals[i]));
                    (*var).setEnqueued(true);
                }
            } */
        }
        numOfSatClauses++;
    }

    std::set<int> copy2 = *clausesToUpdate;
    
    for (auto clauseIndex2 = copy2.begin(); clauseIndex2 != copy2.end(); ++clauseIndex2) {
        clauses[*clauseIndex2].decrementActive();
        if (clauses[*clauseIndex2].getSatisfiedBy() != 0) continue;
        Clause* clause = &clauses[*clauseIndex2];

        if (clause->getActive() == 0) {
            backtrackFlag = true;
        }
        if (clauses[*clauseIndex2].getActive() == 1) {
            for (int i = 0; i < clause->literals.size(); i++) {
                if (vars[std::abs(clause->literals[i])].getValue() == FREE && 
                   !vars[std::abs(clause->literals[i])].getEnqueued()) {
                    //printf("Pushing %i\n", clause->literals[i]);
                    toPropagate.push(clause->literals[i]);
                    vars[std::abs(clause->literals[i])].setEnqueued(true);
                }
            }
        }
    }

    //delete clausesToUpdate;

    //delete clausesToMarkSatisfied;

    if (backtrackFlag) backtrack();

    // All clauses are sat and there are no more pure/unit literals threatening that? => SAT!
    if (toPropagate.empty() && numOfSatClauses == numOfClauses) pthread_exit(0);

}