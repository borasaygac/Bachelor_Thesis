// this .hpp deals with the variables and their representation 

#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <vector>
#include <set>

using namespace std;

enum Assignment {
    FALSE,
    TRUE,
    FREE
};

class Variable {
private:
    Assignment value;
    int index;
    int pos_occ;
    int neg_occ;
    int tot_occ;
    set<int> dynamicOccursPosInClauses; // initially a copy of occursPosInClauses. Needed for DPll
    set<int> occursPosInClauses;
    set<int> occursNegInClauses;
    set<int> dynamicOccursNegInClauses; // initially a copy of occursNegInClauses. Needed for DPLL
    bool enqueued;
    bool forced;
public:
    Variable() {
        value = FREE;
        pos_occ = 0;
        neg_occ = 0;
        tot_occ = 0;
        enqueued = false;
        forced = false;
    };

    void setIndex(int i);
    int const getIndex() {return index;};

    void incPosOcc();
    void incNegOcc();

    void decPosOcc();
    void decNegOcc();

    void updateTotOcc();

    void addOccsToClause(int clause);

    void copyOccsToDynOccs();

    set<int>& getPositiveOccurrances() {return occursPosInClauses;};
    set<int>& getNegativeOccurrances() {return occursNegInClauses;};
    int getTotalOccurances() {return tot_occ;};

    void setDynamicPositiveOccurrances(set<int> dynPos) {dynamicOccursPosInClauses = dynPos;};
    void setDynamicNegativeOccurrances(set<int> dynNeg) {dynamicOccursNegInClauses = dynNeg;};

    set<int>& getDynamicPositiveOccurrances() {return dynamicOccursPosInClauses;};
    set<int>& getDynamicNegativeOccurrances() {return dynamicOccursNegInClauses;};

    Assignment const getValue() {return value;};
    void setValue(Assignment val) {value = val;};

    void setEnqueued(bool enq) {enqueued = enq;};
    bool const getEnqueued() {return enqueued;};

    void setForced(bool f) {forced = f;};
    bool const getForced() {return forced;};
};

#endif // VARIABLE_HPP
