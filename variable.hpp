// this .hpp deals with the variables and their representation 

#include <vector>

using namespace std;



class Variable {
private:
    int pos_occ;
    int neg_occ;
    int tot_occ;
    vector<int> occursInClauses;
public:
    Variable();
    ~Variable();
    void incPosOcc();
    void incNegOcc();
    void decPosOcc();
    void decNegOcc();
    void updateTotOcc();
};

