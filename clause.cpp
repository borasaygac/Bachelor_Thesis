#include "clause.hpp"
#include "globals.hpp"

#include <cstdlib>

using namespace std;

Clause::~Clause()
{
}


void Clause::clauseAddElem(int v)
{

    this->elements.push_back(vars[abs(v)]);

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

void const printHornClauses() {   // Print the Horn clauses for debugging purposes

    for (int i = 1; i < numOfClauses + 1; i++) {
        printf("Clause %i: Horn = %s\n", clauses[i].getIndex(), clauses[i].getIsHorn() ? "true" : "false" );
    }
    printf("Horn Clauses: "); 

    for (int i = 1; i < numOfClauses + 1; i++){

        if(clauses[i].getIsHorn()) {
            printf("%i ", i);
        }
    }
}
// Check if the formula is Horn

bool const isHornFormula(int numOfClauses, vector<Clause> clauses){

    int counter = 0;

    for (int i = 1; i <= numOfClauses; i++) { // Check if all clauses are Horn
        bool isClauseHorn = clauses[i].IsClauseHorn(i);
        
        if (isClauseHorn) counter++;
    }

    if (counter == numOfClauses) {
        printf("The formula is Horn.\n");
        return true;
    } else {
        printf("The formula is not Horn.\n");
        return false;
    }
}
