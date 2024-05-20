// Clause structure and related operations

#include <vector>
#include "variable.hpp"

class Clause{

private:
    std::vector<Variable> elements;
    bool isHorn;
    int clausePosOcc;
    int clauseNegOcc;
    
public:
    Clause();
    ~Clause();
    void clauseAddElem(int v);
    int getElemsSize();
};
