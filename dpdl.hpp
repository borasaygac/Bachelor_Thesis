// This file holds the declarations of the functions used in the DPDL algorithm.

#ifndef DPDL_HPP
#define DPDL_HPP

#include <stack>
#include <queue>

extern int numOfSatClauses;

extern bool backtrackFlag;

extern int btc; // backtrack counter

extern int curVar;

extern int dc; // decision counter

extern std::stack<int> assig;

extern std::queue<int> toPropagate;

void chooseINC();

void propagate();

void update(int assertedVar);

void backtrack();

void revert(int unassignedVar);

#endif // DPDL_HPP