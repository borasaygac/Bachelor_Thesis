#include "../../globals.hpp"
#include "../../dpdl.hpp"

#include <set>
#include <algorithm>

void update(int assertedVar) {
    // clauses where assertedVar evaluates to FALSE
    std::set<int>* clausesToUpdate;

    // clauses where assertedVar evaluates to TRUE
    std::set<int>* clausesToMarkSatisfied;

    // In DPDL - analogous to UP - if the var is true than the negative occurrances need to be updated as they will evaluate to false
    // vice versa otherwise
    if (vars[assertedVar].getValue() == TRUE) { 
        clausesToUpdate->insert(vars[assertedVar].getNegativeOccurrances().begin(), vars[assertedVar].getNegativeOccurrances().end());
    } else {
        clausesToUpdate->insert(vars[assertedVar].getPositiveOccurrances().begin(), vars[assertedVar].getPositiveOccurrances().end());
    }

    // Similarly, the clauses to mark satisfied are the ones are the positives if the var is true, and vice versa 
    if (vars[assertedVar].getValue() == TRUE) {
        clausesToMarkSatisfied->insert(vars[assertedVar].getDynamicPositiveOccurrances().begin(), vars[assertedVar].getDynamicPositiveOccurrances().end());
    } else {
        clausesToMarkSatisfied->insert(vars[assertedVar].getDynamicNegativeOccurrances().begin(), vars[assertedVar].getDynamicNegativeOccurrances().end());
        assertedVar = -assertedVar;
    }

    std::set<int> copy = *clausesToMarkSatisfied;

    // While clauses to mark satisfied are unsatisfied, mark satisfied and
    // erase all references of the literals occuring in the clause, since they can be disregarded
    for (auto clauseIndex = copy.begin(); clauseIndex != copy.end(); ++clauseIndex) {
        Clause* clause = &clauses[*clauseIndex];
        clause->setSatisfiedBy(assertedVar);

        for (int i = 0; i < clause->getElemsSize(); i++) {
            auto var = &vars[std::abs(clause->getElems().at(i)->getIndex())];

            // remove the clause from the dynamic occurrances of the variable according to the sign of the literal
            if (cnf[clause->getIndex()][i] > 0) {
                auto occurrances = var->getDynamicPositiveOccurrances(); 
                auto it = std::find(occurrances.begin(), occurrances.end(), *clauseIndex);
                if (it != occurrances.end()) {
                    occurrances.erase(it);
                }
                var->setDynamicPositiveOccurrances(occurrances);
            } else {
                auto occurrances = var->getNegativeOccurrances(); 
                auto it = std::find(occurrances.begin(), occurrances.end(), *clauseIndex);
                if (it != occurrances.end()) {
                    occurrances.erase(it);
                }
                var->setDynamicNegativeOccurrances(occurrances);
            }
            

            // detect pure literals
            if ((*var).getValue() == FREE && !(*var).getEnqueued()) {
                if ((*var).getDynamicPositiveOccurrances().size() == 0 && (*var).getDynamicNegativeOccurrances().size() > 0) {
                    toPropagate.push(-std::abs(clause->getElems().at(i)->getIndex()));
                    (*var).setEnqueued(true);
                }
                if ((*var).getDynamicNegativeOccurrances().size() == 0 && (*var).getDynamicPositiveOccurrances().size() > 0) {
                    toPropagate.push(std::abs(clause->getElems().at(i)->getIndex()));
                    (*var).setEnqueued(true);
                }
            }
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
            for (int i = 0; i < clause->getElemsSize(); i++) {
                if (vars[std::abs(clause->getElems().at(i)->getIndex())].getValue() == FREE && 
                   !vars[std::abs(clause->getElems().at(i)->getIndex())].getEnqueued()) {
                    toPropagate.push(cnf[*clauseIndex2][clause->getElems().at(i)->getIndex()]);
                    vars[std::abs(clause->getElems().at(i)->getIndex())].setEnqueued(true);
                }
            }
        }
    }

    if (backtrackFlag) backtrack();

    // All clauses are sat and there are no more pure/unit literals threatening that? => SAT!
    if (toPropagate.empty() && numOfSatClauses == numOfClauses) pthread_exit(0);

}