#include "../../globals.hpp"
#include "../../dpdl.hpp"

#include <cmath>
#include <thread>
#include <future>

void backtrack() {
  backtrackFlag = false;

  // unassign until the last branching variable.
  while (!assig.empty() && vars[assig.top()].getForced()) {
    btc++;
    int toUnassign = assig.top();
    revert(toUnassign);
    vars[toUnassign].setValue(FREE);
    vars[toUnassign].setForced(false);

    assig.pop();
  }

  // clear queue of unit and pure literals
  while (!toPropagate.empty()) {
    vars[std::abs(toPropagate.front())].setEnqueued(false);
    toPropagate.pop();
  }

  // UNSAT
  // if no more variables to unassign, we have backtracked to the first decision
  // terminate thread and return 1
  if (assig.empty()) {
    pthread_exit((void *)1);
  }

  // handle most recent branching variable
  int b = assig.top();
  revert(b);
  vars[b].setForced(true);

  // assign negated val
  vars[b].setValue(Assignment(int(2 - std::pow(2.0, vars[b].getValue()))));
  curVar = b;
  update(b);
  propagate();
}