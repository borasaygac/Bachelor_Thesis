#include "clause.hpp"
#include "globals.hpp"

#include <cstdlib>

using namespace std;

Clause::~Clause()
{
}


void Clause::clauseAddElem(int v)
{

    this->elements.push_back(&vars[abs(v)]);

    if (v > 0)
    {
        this->clausePosOcc++;
        vars[abs(v)].addOccsToClause(this->index);
    }
    else
    {
        this->clauseNegOcc++;
        vars[abs(v)].addOccsToClause(this->index * -1);
    }
}


bool Clause::IsClauseHorn(int index)
{ // A clause is Horn if it has at most one positive literal

    if (clauses[index].clausePosOcc <= 1)
    {
        clauses[index].isHorn = true;
    } else {
        clauses[index].isHorn  = false;
    }
    return clauses[index].isHorn ;
}

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

bool const Clause::evaluateClause() {
    for (int i = 0; i < this->elements.size(); i++) {
        if (((*this->elements[i]).getValue() == TRUE && cnf[this->index][i] > 0) || 
            ((*this->elements[i]).getValue() == FALSE && cnf[this->index][i] < 0)) { 
            // if the variable is true and the literal is positive => clause satisfied or
            // if the variable is false and the literal is negative => clause satisfied
            return true;
        }
    }
    // if none of the above conditions are met => clause not satisfied
    return false;
}
