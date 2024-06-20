#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <vector>
#include "variable.hpp"
#include "clause.hpp"
#include "parse.hpp"


using namespace std;

// General Global Variables

extern int numOfVars;

extern int numOfClauses;

extern vector<Variable> vars;

extern vector<Clause> clauses;

extern vector<vector<int>> cnf;

// Unit Clause Global Variables

extern int unitClauseCount;

extern vector<Clause> unitClauses;

// Horn Solver Global Variables

int hornSolver();


// 2-SAT Global Variables and Functions

bool isTwoSat(int numOfClauses, vector<Clause> clauses);

// Non Interlaced SAT Global Variables

extern vector<pair<int,int>> deltaF;

void createDeltaF();

bool isFNonInterlaced();

// Print Related Functions

void const printModel(bool sat);

// Verify if the formula is satisfied and related functions

bool isFormulaSat();


#endif // GLOBALS_HPP