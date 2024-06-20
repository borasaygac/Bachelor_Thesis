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
    int pos_occ;
    int neg_occ;
    int tot_occ;
    vector<int> occursPosInClauses;
    vector<int> dynamicOccursPosInClauses; // initially a copy of occursPosInClauses. Needed for 2-SAT
    vector<int> occursNegInClauses;
    vector<int> dynamicOccursNegInClauses; // initially a copy of occursNegInClauses. Needed for 2-SAT
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

    ~Variable();
    void setIndex(int i);

    void incPosOcc();
    void incNegOcc();

    void decPosOcc();
    void decNegOcc();

    void updateTotOcc();

    void addOccsToClause(int clause);

    void copyOccsToDynOccs();

    vector<int> getPositiveOccurrances() {return occursPosInClauses;};
    vector<int> getNegativeOccurrances() {return occursNegInClauses;};

    vector<int> getDynamicPositiveOccurrances() {return dynamicOccursPosInClauses;};
    vector<int> getDynamicNegativeOccurrances() {return dynamicOccursNegInClauses;};

    Assignment getValue() {return value;};
    void setValue(Assignment val) {value = val;};

    void setEnqueued(bool enq) {enqueued = enq;};
    bool getEnqueued() {return enqueued;};

    void setForced(bool f) {forced = f;};
    bool getForced() {return forced;};
};


#endif // VARIABLE_HPP

