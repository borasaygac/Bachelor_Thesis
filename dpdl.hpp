#ifndef DPDL_HPP
#define DPDL_HPP

#include <stack>
#include <queue>

std::stack<int> assig;

std::queue<int> toPropagate;

void propagate();

void update(int assertedVar);

#endif // DPDL_HPP