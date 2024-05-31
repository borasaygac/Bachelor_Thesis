#include "globals.hpp"
#include <iostream>


bool isFormulaSat() {
    for (int i = 1; i <= numOfClauses; i++) {
        if (!clauses[i].evaluateClause()) {
            return false;
        }
    }
    return true;
}

void const printModel(bool sat) {
  if (!sat) {
    printf("\033[31mUNSAT: No model!\033[0m\n\n");}
    // for (int i = 1; i <= numOfVars; i++) {
    //   int value = 0;
    //   if (vars[i].getValue() == FREE)
    //     value = 0;
    //   if (vars[i].getValue() == TRUE)
    //     value = i;
    //   if (vars[i].getValue() == FALSE)
    //     value = -i;

    //   printf("%i", value);
    //   (i < numOfVars) ? printf(", ", value) : printf("]\033[0m\n");
    //   if (i % 20 == 0)
    //     printf("\n");
    // }
   else {
    cout << "\033[1;32mSAT!";
    printf(" Model:\n\n[");
    for (int i = 1; i <= numOfVars; i++) {
      int value = 0;
      if (vars[i].getValue() == FREE)
        value = 0;
      if (vars[i].getValue() == TRUE)
        value = i;
      if (vars[i].getValue() == FALSE)
        value = -i;

      printf("%i", value);
      (i < numOfVars) ? printf(", ", value) : printf("]\033[0m\n");
      if (i % 20 == 0)
        printf("\n");
    }
  }
}


