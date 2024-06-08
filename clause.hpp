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
    int clausePosOcc;
    int clauseNegOcc;
    
public:
    Clause() {
        this->isHorn = false;
        this->clausePosOcc = 0;
        this->clauseNegOcc = 0;
    };
    ~Clause();
    void setIndex(int i) { index = i; }
    void clauseAddElem(int v);
    int const getElemsSize() { return elements.size(); }
    bool IsClauseHorn(int index);
    bool const getIsHorn() { return isHorn; }
    int const getIndex() { return index; }
    bool const evaluateClause();
    bool const contains(int v);
};


bool const isHornFormula(int numOfClauses, std::vector<Clause> clauses);

void const printHornClauses(bool horn);



#endif // CLAUSE_HPP