#ifndef DPDL_HPP
#define DPDL_HPP

#include <stack>
#include <queue>

extern int numOfSatClauses;

extern bool backtrackFlag;

extern int btc; // backtrack counter

extern int curVar;

std::stack<int> assig;

std::queue<int> toPropagate;

void propagate();

void update(int assertedVar);

void backtrack();

void revert(int unassignedVar);

#endif // DPDL_HPP