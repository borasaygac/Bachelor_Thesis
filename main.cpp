#include <iostream>
#include <stdexcept>
#include <chrono>
#include <string>
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
vector<pair<int,int>> deltaF;
bool horn = false;

int main (int argc, char *argv[]) {

    if (argc < 2 ){
        cerr << "\nUsage: " << argv[0] << " <filename>\n" << "Please add a file when calling the main.exe!\n" << endl;
        return 1;
    }

    // measure CPU time...
    chrono::steady_clock::time_point start = chrono::steady_clock::now();

    string fileName = argv[1];
    try {
        parseDIMACS(fileName);
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        // handle error 
    }

    horn = isHornFormula(numOfClauses, clauses); // check if the formula is a horn formula

    printHornClauses(horn); // print the non-horn clauses if formula not horn

    // Add non interlaced check here
    // deltaF

    if (horn) {
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
    }
    
    
    createDeltaF(); // create deltaF vector

    //print deltaF

    printf("DeltaF: ");
    for (pair<int,int> p : deltaF) {
        printf("(%i, %i), ", p.first, p.second);
    }
    printf("\n");

    isFNonInterlaced(); // check if the formula is non-interlaced

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