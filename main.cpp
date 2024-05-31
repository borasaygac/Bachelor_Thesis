#include <iostream>
#include <stdexcept>
#include <chrono>
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

    // measure CPU time...
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    string fileName = "horntest02.cnf";
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

    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    chrono::duration<double> duration = chrono::duration_cast<std::chrono::duration<double>>(end - start);


    // TODO: For 2 sat there is an easier way to solve it, without building the graph

    printModel(sat);

    printf("\nCPU time used: %.6f seconds\n\n", duration.count());

    cout.flush();

    printf("-------------------------------------\n\n");


    return 0;
}