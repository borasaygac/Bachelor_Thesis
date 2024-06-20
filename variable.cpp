#include "variable.hpp"
#include <cstdlib>

/*Variable::Variable(){ // constructor
    this -> pos_occ = 0;
    this -> neg_occ = 0;
    this -> tot_occ = 0; 
}*/

Variable::~Variable(){ // destructor

}

void Variable::setIndex(int i) { // set the index of the variable (1-indexed to n= numOfVars) 
    this -> index = i;
}

// Number of occurance based operations

void Variable::updateTotOcc() {  // update the total occurance of the variable
    this -> tot_occ = pos_occ + neg_occ;
} 

void Variable::incPosOcc() { // increase the positive occurance of the variable
    this -> pos_occ++;
    updateTotOcc();
} 

void Variable::decPosOcc() { // decrease the positive occurance of the variable
    this -> pos_occ--;
    updateTotOcc();
} 

void Variable::incNegOcc() { // increase the negative occurance of the variable
    this -> neg_occ++;
    updateTotOcc();
} 

void Variable::decNegOcc() { // decrease the negative occurance of the variable
    this -> neg_occ--;
    updateTotOcc();
} 

void Variable::addOccsToClause(int clause) { // adds which clauses the variable occurs in, positive or negative respectively 
    if (clause > 0) {
        occursPosInClauses.insert(clause);
    } else {
        occursNegInClauses.insert(abs(clause));
    }
}

void Variable::copyOccsToDynOccs() {
    this -> dynamicOccursPosInClauses = occursPosInClauses;
    this -> dynamicOccursNegInClauses = occursNegInClauses;
}