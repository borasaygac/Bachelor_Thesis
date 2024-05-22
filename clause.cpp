#include "clause.hpp"
#include "globals.hpp"


Clause::Clause(){
    this -> isHorn = false;
    this -> clausePosOcc = 0;
    this -> clauseNegOcc = 0;
}

Clause::~Clause(){
}

void Clause::clauseAddElem(int v){
    this -> elements.push_back(vars[v]);
    //TODO: Fix this
}


int Clause::getElemsSize(){
    return this -> elements.size();
}