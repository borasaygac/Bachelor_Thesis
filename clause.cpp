#include "clause.hpp"
#include "globals.hpp"

#include <cstdlib>

using namespace std;

Clause::Clause(){
    this -> isHorn = false;
    this -> clausePosOcc = 0;
    this -> clauseNegOcc = 0;
}

Clause::~Clause(){
}

void Clause::setIndex(int i){
    this -> index = i;
}

void Clause::clauseAddElem(int v){
    
    this -> elements.push_back(vars[abs(v)]);

    if (v > 0) {
        this -> clausePosOcc++;
        vars[abs(v)].addOccsToClause(this -> index);
    } else {
        this -> clauseNegOcc++;
        vars[abs(v)].addOccsToClause(this -> index*-1);
    } 
}


int Clause::getElemsSize(){
    return this -> elements.size();
}

bool Clause::IsClauseHorn(){   // A clause is Horn if it has at most one positive literal
    if (this -> clausePosOcc <= 1 && (this -> getElemsSize() > 0)){
        this -> isHorn = true;
    }
    return this -> isHorn;
}

// Check if the formula is Horn

bool isHornFormula(int numOfClauses, vector<Clause> clauses){
    for (int i = 1; i < numOfClauses; i++){
        if (!clauses[i].IsClauseHorn()){
            return false;
        }
    }
    return true;
}