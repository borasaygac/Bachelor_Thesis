// this .hpp deals with the variables and their representation 

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <vector>

using namespace std;

enum Assignment {FREE, TRUE, FALSE};

class Variable {
private:
    Assignment value;
    int index;
    int pos_occ = 0;
    int neg_occ = 0;
    int tot_occ = 0;
    vector<int> occursPosInClauses;
    vector<int> occursNegInClauses;
public:
    Variable();
    ~Variable();
    void setIndex(int i);
    void incPosOcc();
    void incNegOcc();
    void decPosOcc();
    void decNegOcc();
    void updateTotOcc();
    void addOccsToClause(int clause);
    Assignment getValue() {return value;};
    void setValue(Assignment val) {value = val;};
};


#endif // VARIABLE_HPP

