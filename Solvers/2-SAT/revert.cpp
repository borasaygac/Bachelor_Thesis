#include "../../globals.hpp"
#include "../../dpdl.hpp"

#include <set>

void revert(int unassignedVar) {
    // clauses where assertedVar evaluates to FALSE
    std::set<int>* clausesToIncrement;

    // all clauses the unassignedVar appears in (including satisfied clauses) and evaluates to TRUE
    std::set<int>* allOccurences;

    if(vars[unassignedVar].getValue() == TRUE) {
        allOccurences->insert(vars[unassignedVar].getPositiveOccurrances().begin(),
                              vars[unassignedVar].getPositiveOccurrances().end());
    } else {
        allOccurences->insert(vars[unassignedVar].getNegativeOccurrances().begin(),
                              vars[unassignedVar].getNegativeOccurrances().end());
    }

    if (vars[unassignedVar].getValue() == TRUE) {
        clausesToIncrement->insert(vars[unassignedVar].getNegativeOccurrances().begin(),
                                   vars[unassignedVar].getNegativeOccurrances().end());
    } else {
        clausesToIncrement->insert(vars[unassignedVar].getPositiveOccurrances().begin(),
                                   vars[unassignedVar].getPositiveOccurrances().end());

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

        for (int i = 0; i < clause->getElemsSize(); i++) {

            if (cnf[*clauseIndex][clause->getElems().at(i)->getIndex()] > 0) {
                auto dynOccurances = vars[std::abs(clause->getElems().at(i)->getIndex())].getDynamicPositiveOccurrances();
                dynOccurances.push_back(*clauseIndex);
                vars[std::abs(clause->getElems().at(i)->getIndex())].setDynamicPositiveOccurrances(dynOccurances);
            } else {
                auto dynOccurances = vars[std::abs(clause->getElems().at(i)->getIndex())].getDynamicNegativeOccurrances();
                dynOccurances.push_back(*clauseIndex);
                vars[std::abs(clause->getElems().at(i)->getIndex())].setDynamicNegativeOccurrances(dynOccurances);
            }
        }
        numOfSatClauses--;
    }
}
