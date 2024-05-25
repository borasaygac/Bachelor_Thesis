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

int main () {

    string fileName = "001_count4_2_s.cnf";
    try {
        parseDIMACS(fileName);
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << '\n';
        // handle error 
    }

    isHornFormula(numOfClauses, clauses);

    printHornClauses();

    // todo: Horn sat solver

    // TODO: For 2 sat there is an easier way to solve it, without building the graph

    return 0;
}