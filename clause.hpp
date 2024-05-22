// Clause structure and related operations

#ifndef CLAUSE_HPP
#define CLAUSE_HPP

#include <vector>
#include "variable.hpp"

class Clause{

private:
    int index;
    std::vector<Variable> elements;
    bool isHorn;
    int clausePosOcc;
    int clauseNegOcc;
    
public:
    Clause();
    ~Clause();
    void setIndex(int i);
    void clauseAddElem(int v);
    int getElemsSize();
    bool IsClauseHorn();
};


bool isHornFormula(int numOfClauses, vector<Clause> clauses);

#endif // CLAUSE_HPP
