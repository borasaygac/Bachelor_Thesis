// Clause structure and related operations

#ifndef CLAUSE_HPP
#define CLAUSE_HPP

#include <vector>
#include "variable.hpp"

class Clause{

private:
    int index;
    std::vector<Variable*> elements;
    bool isHorn;
    bool istwoSat;
    int clausePosOcc;
    int clauseNegOcc;
    int satisfiedBy;
    int active;
    
public:
    Clause() {
        this->isHorn = false;
        this->istwoSat = false;
        this->clausePosOcc = 0;
        this->clauseNegOcc = 0;
        this->satisfiedBy = 0;
        this -> active = 0;
    };
    ~Clause();

    void setIndex(int i) { index = i;};
    int const getIndex() { return index;};

    void clauseAddElem(int v);
    int const getElemsSize() { return elements.size();};
    vector<Variable*> const getElems() {return elements;};

    void incrementActive() { active++;};
    void decrementActive() { active--;};
    int getActive() { return active;};

    bool IsClauseHorn(int index);
    bool const getIsHorn() { return isHorn;};


    bool IsClauseTwoSat(int index);

    bool const evaluateClause();

    bool const contains(int v);

    void setSatisfiedBy(int v) { satisfiedBy = v;};
    int getSatisfiedBy() { return satisfiedBy;};
};


bool const isHornFormula(int numOfClauses, std::vector<Clause> clauses);

void const printHornClauses(bool horn);



#endif // CLAUSE_HPP