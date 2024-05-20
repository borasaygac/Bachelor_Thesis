// this .hpp deals with the variables and their representation 

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <vector>

using namespace std;

class Variable {
private:
    int pos_occ;
    int neg_occ;
    int tot_occ;
    vector<int> occursPosInClauses;
    vector<int> occursNegInClauses;
public:
    Variable();
    ~Variable();
    void incPosOcc();
    void incNegOcc();
    void decPosOcc();
    void decNegOcc();
    void updateTotOcc();
};


#endif // VARIABLE_HPP

