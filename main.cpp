#include <iostream>
#include <stdexcept>
#include "parse.hpp"
#include "clause.hpp"

using namespace std;

int numOfVars = 0;
int numOfClauses = 0;
vector<Variable> vars;
vector<Clause> clauses;
vector<vector<int>> cnf;
int unitClauseCount = 0;
vector<Clause> unitClauses;

int main () {

    string fileName = "002_testunit.cnf";
    try {
        parseDIMACS(fileName);
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        // handle error 
    }

    isHornFormula(numOfClauses, clauses);

    printHornClauses();

    //hornSolver();
    printf("Unit Clause Count: %i\n", unitClauseCount);
    printf("Unit Clauses: ");
    if (unitClauses.empty()){
        printf("No unit clauses!\n");
    } else {
        for (Clause tmp_cls : unitClauses) {
            printf("%i, ", tmp_cls.getIndex());
        }
        printf("\n");
    }
    
    hornSolver();

    printHornAssignments();

    //print var assigs
    for (int i = 1; i <= numOfVars; i++) {
        printf("Variable %i: %i\n", i, vars[i].getValue());
    }

    bool sat = isFormulaSat();

    if (sat) {
        printf("The formula is SAT\n");
    } else {
        printf("The formula is UNSAT\n");
    }

    // TODO: For 2 sat there is an easier way to solve it, without building the graph

    return 0;
}