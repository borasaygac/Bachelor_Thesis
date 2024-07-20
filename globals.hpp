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

void* twoSatDPDL(void* arg);

// Nested SAT Global Variables

extern vector<vector<int>> orderedCNF;

extern vector<int> lits;

extern vector<int> start;

void fillLiteralsAndStart();

void createOrderedCNF();

bool isNested();

void nestedSolver();

vector<int> nestedClausePreOrder(vector<int> &clause);

bool doesClauseOneStraddleClauseTwo(vector<int> &clauseOne, vector<int> &clauseTwo);

bool doesClauseTwoStraddleClauseOne(vector<int> &clauseOne, vector<int> &clauseTwo);

// Co-Nested SAT Global Variables

void copyCNF();

void callPythonScript();

// Non Interlaced SAT Global Variables

extern vector<pair<int,int>> deltaF;

void createDeltaF();

bool isFNonInterlaced();

// Print Related Functions

void const printModel(bool sat);

// Verify if the formula is satisfied and related functions

bool isFormulaSat();


#endif // GLOBALS_HPP