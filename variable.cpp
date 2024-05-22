#include "variable.hpp"

Variable::Variable(){ // constructor
    this -> pos_occ = 0;
    this -> neg_occ = 0;
    this -> tot_occ = 0; 
}

Variable::~Variable(){ // destructor

}

void Variable::setIndex(int i) {
    this -> index = i;
}

// Number of occurance based operations

void Variable::updateTotOcc() {
    this -> tot_occ = pos_occ + neg_occ;
} 

void Variable::incPosOcc() {
    this -> pos_occ++;
    updateTotOcc();
} 

void Variable::decPosOcc() {
    this -> pos_occ--;
    updateTotOcc();
} 

void Variable::incNegOcc() {
    this -> neg_occ++;
    updateTotOcc();
} 

void Variable::decNegOcc() {
    this -> neg_occ--;
    updateTotOcc();
} 

void Variable::addOccsToClause(int clause) {
    if (clause > 0) {
        occursPosInClauses.push_back(clause);
    } else {
        occursNegInClauses.push_back(clause);
    }
}
