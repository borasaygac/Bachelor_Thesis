#include "globals.hpp"
#include <algorithm>



void createDeltaF() {
    // Let F be a formula
    // Let Delta(F) be the set of variables where [i,j] are indexes of clauses in F and there exists an i, j such that i < j
    // and there exists a variable x in clause i and the negation of the variable -x in clause j.

    // for each clause i in F
    for (int i = 1; i <= numOfClauses; i++) {
        // for each element in clause i
        for (int j = 0; j < clauses[i].getElemsSize(); j++) {
            // look for other clauses that have the negation of the element in clause i
            for (int k = i + 1; k <= numOfClauses; k++) {
                std::pair<int, int> tmpPair = make_pair(i, k);
                if (clauses[k].contains(-cnf[clauses[i].getIndex()][j])) {
                    // if the negation of the element in clause i is found in clause k and no duplicate is found
                    
                    if ((std::find(deltaF.begin(), deltaF.end(), tmpPair)) == deltaF.end()) {
                        deltaF.push_back(tmpPair); // add the pair of indexes to deltaF
                    }
                }
            }
        }
    }
}

bool isFNonInterlaced() {
    // Let F be a formula
    // F is interlaced, iff there exists two [i,j] and [k,l] in Delta(F) such that i < k < j < l
    // where the pairs are elements of DeltaF. 
    // Otherwise F is non-interlaced.

    // deltaF[i].first == i 
    // deltaF[i].second == j
    // deltaF[j].first == k
    // deltaF[j].second == l

    for (int i = 0; i < deltaF.size(); i++) {
        for (int j = i + 1; j < deltaF.size(); j++) {
            if (deltaF[i].first < deltaF[j].first && 
                deltaF[j].first < deltaF[i].second && 
                deltaF[i].second < deltaF[j].second) {
                printf("F is interlaced\n");
                printf("(i: %i, j: %i), (k: %i, l: %i)\n", deltaF[i].first, deltaF[i].second, deltaF[j].first, deltaF[j].second);
                printf("%i < %i < %i < %i\n", deltaF[i].first, deltaF[j].first, deltaF[i].second, deltaF[j].second);
                printf("\n\n");
                return false;
            }
        }
    }

    printf("F is non-interlaced\n");
    printf("\n\n");
    return true;
}