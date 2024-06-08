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

// Check if the clause contains a literal v
bool const Clause::contains(int v) {
    for (int i = 0; i < this->elements.size(); i++) { // within the elements of the clause
        if (cnf[this->index][i] == v ) { // if the literal is found 
            // The signage has to match -> if v is positive, only query for positive v and not negative v
            return true;
        }
    }
    // else return false
    return false;
}
