#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <vector>
#include "variable.hpp"
#include "clause.hpp"
#include "parse.hpp"

using namespace std;

extern int numOfVars;

extern int numOfClauses;

extern vector<Variable> vars;

extern vector<Clause> clauses;

extern vector<vector<int>> cnf;

extern int unitClauseCount;

extern vector<Clause> unitClauses;

int hornSolver();

int printHornAssignments();


#endif // GLOBALS_HPP