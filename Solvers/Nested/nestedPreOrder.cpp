#include <cstdlib>
#include <algorithm>
#include "../../globals.hpp"


struct nestedLess {
    bool operator()(int a, int b) const {
        return abs(a) < abs(b);
    }
};

vector<int> nestedPreOrder(vector<int> &clause) {
    std::vector<int> preOrder(clause.size());
    preOrder = clause;
    std::sort(preOrder.begin(), preOrder.end(), nestedLess());

    for(auto elem: clause){
        printf("%i ", elem);
    }
    printf(" || ");

    for(auto elem: preOrder){
        printf("%i ", elem);
    }
    printf("\n");

    return preOrder;
}

