#include "../../globals.hpp"
#include "../../dpdl.hpp"

#include <set>

void revert(int unassignedVar) {
    // clauses where assertedVar evaluates to FALSE
    std::set<int>* clausesToIncrement;

    // all clauses the unassignedVar appears in (including satisfied clauses) and evaluates to TRUE
    std::set<int>* allOccurences;

    allOccurences = (vars[unassignedVar].getValue() == TRUE) ? &vars[unassignedVar].getPositiveOccurrances(): 
                                                               &vars[unassignedVar].getNegativeOccurrances();

    if (vars[unassignedVar].getValue() == TRUE) {
        clausesToIncrement = &vars[unassignedVar].getNegativeOccurrances();
    } else {
        clausesToIncrement = &vars[unassignedVar].getPositiveOccurrances();
        unassignedVar = -unassignedVar;
    }

    std::set<int> copy2 = *clausesToIncrement;

    for (auto clauseIndex2 = copy2.begin(); clauseIndex2 != copy2.end(); ++clauseIndex2) 
        clauses[*clauseIndex2].incrementActive();


    std::set<int> copy = *allOccurences;

    // restore the previously deleted clauses and its var references
    for (auto clauseIndex = copy.begin(); clauseIndex != copy.end(); ++clauseIndex) {
        if (clauses[*clauseIndex].getSatisfiedBy() != unassignedVar) continue;

        Clause* clause = &clauses[*clauseIndex];

        clause->setSatisfiedBy(0);

        for (int i = 0; i < clause->literals.size(); i++) {
            clause -> literals[i] > 0 ? vars[std::abs(clause->literals[i])].getDynamicPositiveOccurrances().insert(*clauseIndex):
                                        vars[std::abs(clause->literals[i])].getDynamicNegativeOccurrances().insert(*clauseIndex);

        }
        numOfSatClauses--;
    }
}
